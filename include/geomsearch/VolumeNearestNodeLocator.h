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
 * Finds the nearest node to each node in block to each node in boundary and the other way around.
 * @todo TODO: the latter is not really necessary but comes from of the algorithm of NearestNodeLocator
 * This code is (with a few relevant modifications) identical to NearestNodeLocator.h/.C
 */
class VolumeNearestNodeLocator : public Restartable
{
public:
  VolumeNearestNodeLocator(SubProblem & subproblem, MooseMesh & mesh, BoundaryID boundary, SubdomainID block);

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
   * Returns the list of block nodes this Locator is tracking.
   */
  std::vector<unsigned int> & blockNodes() { return _block_nodes; }

  /**
   * Returns the NodeIdRange of block nodes to be used for calling threaded
   * functions operating on the block nodes.
   */
  NodeIdRange & blockNodeRange() { return *_block_node_range; }

protected:
  SubProblem & _subproblem;

  MooseMesh & _mesh;

  NodeIdRange * _block_node_range;

public:
  std::map<unsigned int, NearestNodeLocator::NearestNodeInfo> _nearest_node_info;

  BoundaryID _boundary;
  SubdomainID _block;

  bool _first;
  std::vector<unsigned int> _block_nodes;

  std::map<unsigned int, std::vector<unsigned int> > _neighbor_nodes;

  // The following parameter controls the patch size that is searched for each nearest neighbor
  static const unsigned int _patch_size;

  // The furthest through the patch that had to be searched for any node last time
  Real _max_patch_percentage;
};

#endif //VolumeNearestNodeLocator_H
