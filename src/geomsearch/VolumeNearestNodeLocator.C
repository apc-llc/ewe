/****************************************************************/
/*  derived form NearestNodeLocator.C                           */
/****************************************************************/

#include "VolumeNearestNodeLocator.h"
#include "MooseMesh.h"
#include "SubProblem.h"
#include "SlaveNeighborhoodThread.h"
#include "NearestNodeThread.h"
#include "Moose.h"
// libMesh
#include "libmesh/boundary_info.h"
#include "libmesh/elem.h"
#include "libmesh/plane.h"
#include "libmesh/mesh_tools.h"

std::string _boundaryBlockFuser(BoundaryID boundary, SubdomainID block)
{
  std::stringstream ss;

  ss << boundary << "to" << block;

  return ss.str();
}


VolumeNearestNodeLocator::VolumeNearestNodeLocator(SubProblem & subproblem, MooseMesh & mesh, BoundaryID boundary, SubdomainID block) :
    Restartable(_boundaryBlockFuser(boundary, block), "VolumeNearestNodeLocator", subproblem, 0),
    _subproblem(subproblem),
    _mesh(mesh),
    _block_node_range(NULL),
    _boundary(boundary),
    _block(block),
    _first(true)
{
  //sanity check on ids
  const std::set<BoundaryID>& bids=_mesh.getBoundaryIDs();
  std::set<BoundaryID>::const_iterator sit;
  sit=bids.find(_boundary);
  if (sit == bids.end())
    mooseError("VolumeNearestNodeLocator being created for boundary "<<_boundary<<" and block "<<_block<<", but boundary "<<_boundary<<" does not exist");

  const std::set<SubdomainID>& sids=_mesh.meshSubdomains();
  std::set<SubdomainID>::const_iterator ssit;
  ssit=sids.find(_block);
  if (ssit == sids.end())
    mooseError("VolumeNearestNodeLocator being created for boundary "<<_boundary<<" and block "<<_block<<", but block "<<_block<<" does not exist");
}

VolumeNearestNodeLocator::~VolumeNearestNodeLocator()
{
  delete _block_node_range;
}

void
VolumeNearestNodeLocator::findNodes()
{
  Moose::perf_log.push("VolumeNearestNodeLocator::findNodes()","Solve");

  /**
   * If this is the first time through we're going to build up a "neighborhood" of nodes
   * surrounding each of the block nodes.  This will speed searching later.
   */
  if (_first)
  {
    _first=false;

    // Trial block nodes are all the nodes on the slave side
    // We only keep the ones that are either on this processor or are likely
    // to interact with elements on this processor (ie nodes owned by this processor
    // are in the "neighborhood" of the block node
    std::vector<unsigned int> trial_block_nodes;    // was slave
    std::vector<unsigned int> trial_boundary_nodes; // was master

    // Build a bounding box.  No reason to consider nodes outside of our inflated BB
    MeshTools::BoundingBox * my_inflated_box = NULL;

    std::vector<Real> & inflation = _mesh.getGhostedBoundaryInflation();

    // This means there was a user specified inflation... so we can build a BB
    if (inflation.size() > 0)
    {
      MeshTools::BoundingBox my_box = MeshTools::processor_bounding_box(_mesh, _mesh.processor_id());

      Real distance_x = 0;
      Real distance_y = 0;
      Real distance_z = 0;

      distance_x = inflation[0];

      if (inflation.size() > 1)
        distance_y = inflation[1];

      if (inflation.size() > 2)
        distance_z = inflation[2];

      my_inflated_box = new MeshTools::BoundingBox(Point(my_box.first(0)-distance_x,
                                                         my_box.first(1)-distance_y,
                                                         my_box.first(2)-distance_z),
                                                   Point(my_box.second(0)+distance_x,
                                                         my_box.second(1)+distance_y,
                                                         my_box.second(2)+distance_z));
    }

    // Data structures to hold the Nodal Boundary conditions
    ConstBndNodeRange & bnd_nodes = *_mesh.getBoundaryNodeRange();
    for (ConstBndNodeRange::const_iterator nd = bnd_nodes.begin() ; nd != bnd_nodes.end(); ++nd)
    {
      const BndNode * bnode = *nd;
      BoundaryID boundary_id = bnode->_bnd_id;
      unsigned int node_id = bnode->_node->id();

      // If we have a BB only consider saving this node if it's in our inflated BB
      if (boundary_id == _boundary && (!my_inflated_box || (my_inflated_box->contains_point(*bnode->_node))))
        trial_boundary_nodes.push_back(node_id);
    }

    for (libMesh::Node const *nd = *_mesh.localNodesBegin() ; nd != *_mesh.localNodesEnd(); ++nd)
    {
      const Node * node = nd;
      std::set<SubdomainID> sids = _mesh.getNodeBlockIds(*node);
      std::set<SubdomainID>::const_iterator ssit(sids.find(_block));
      unsigned int node_id = node->id();

      // If we have a BB only consider saving this node if it's in our inflated BB
      if (ssit != sids.end() && (!my_inflated_box || (my_inflated_box->contains_point(*node))))
        trial_block_nodes.push_back(node_id);
    }

    // don't need the BB anymore
    delete my_inflated_box;

    std::map<unsigned int, std::vector<unsigned int> > & node_to_elem_map = _mesh.nodeToElemMap();
    SlaveNeighborhoodThread snt(_mesh, trial_boundary_nodes, node_to_elem_map, _mesh.getPatchSize());

    NodeIdRange trial_block_node_range(trial_block_nodes.begin(), trial_block_nodes.end(), 1);
    Threads::parallel_reduce(trial_block_node_range, snt);

    _block_nodes = snt._slave_nodes;
    _neighbor_nodes = snt._neighbor_nodes;

    for (std::set<unsigned int>::iterator it = snt._ghosted_elems.begin();
        it != snt._ghosted_elems.end();
        ++it)
      _subproblem.addGhostedElem(*it);

    // Cache the blocke_node_range so we don't have to build it each time
    _block_node_range = new NodeIdRange(_block_nodes.begin(), _block_nodes.end(), 1);
  }

  _nearest_node_info.clear();

  NearestNodeThread nnt(_mesh, _neighbor_nodes);

  Threads::parallel_reduce(*_block_node_range, nnt);

  _max_patch_percentage = nnt._max_patch_percentage;

  _nearest_node_info = nnt._nearest_node_info;

  Moose::perf_log.pop("VolumeNearestNodeLocator::findNodes()","Solve");
}

void
VolumeNearestNodeLocator::reinit()
{
  // Reset all data
  delete _block_node_range;
  _block_node_range = NULL;
  _nearest_node_info.clear();

  _first = true;

  _block_nodes.clear();
  _neighbor_nodes.clear();

  // Redo the search
  findNodes();
}

Real
VolumeNearestNodeLocator::distance(unsigned int node_id)
{
  return _nearest_node_info[node_id]._distance;
}


const Node *
VolumeNearestNodeLocator::nearestNode(unsigned int node_id)
{
  return _nearest_node_info[node_id]._nearest_node;
}
