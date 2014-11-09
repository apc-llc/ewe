/****************************************************************/
/* derived from NearestNodeLocator.h                            */
/****************************************************************/

#ifndef VolumeNearestNodeLocator_H
#define VolumeNearestNodeLocator_H

// Moose
#include "MooseMesh.h"
#include "libmesh/vector_value.h"
#include "Restartable.h"
#include "NearestNodeLocator.h"

// libMesh
#include "libmesh/libmesh_common.h"

// System
#include <vector>
#include <map>


class MooseMesh;
class SubProblem;

/**
 * Finds the nearest node to each node in boundary1 to each node in boundary2 and the other way around.
 */
class VolumeNearestNodeLocator : public Restartable
{
public:
  VolumeNearestNodeLocator(SubProblem & subproblem, MooseMesh & mesh, BoundaryID boundary1, BoundaryID boundary2);

  ~VolumeNearestNodeLocator();

  /**
   * This is the main method that is going to start the search.
   */
  void findNodes();

  /**
   * Completely redo the search from scratch.
   * Most likely called because of mesh adaptivity.
   */
  void reinit();

  /**
   * Valid to call this after findNodes() has been called to get the distance to the nearest node.
   */
  Real distance(unsigned int node_id);

  /**
   * Valid to call this after findNodes() has been called to get a pointer to the nearest node.
   */
  const Node * nearestNode(unsigned int node_id);

  /**
   * Returns the list of slave nodes this Locator is tracking.
   */
  std::vector<unsigned int> & slaveNodes() { return _slave_nodes; }

  /**
   * Returns the NodeIdRange of slave nodes to be used for calling threaded
   * functions operating on the slave nodes.
   */
  NodeIdRange & slaveNodeRange() { return *_slave_node_range; }

protected:
  SubProblem & _subproblem;

  MooseMesh & _mesh;

  NodeIdRange * _slave_node_range;

public:
  std::map<unsigned int, NearestNodeLocator::NearestNodeInfo> _nearest_node_info;

  BoundaryID _boundary1;
  BoundaryID _boundary2;

  bool _first;
  std::vector<unsigned int> _slave_nodes;

  std::map<unsigned int, std::vector<unsigned int> > _neighbor_nodes;

  // The following parameter controls the patch size that is searched for each nearest neighbor
  static const unsigned int _patch_size;

  // The furthest through the patch that had to be searched for any node last time
  Real _max_patch_percentage;
};

#endif //VolumeNearestNodeLocator_H
