#include "VolumeNearestNodeDistanceAux.h"

template<>
InputParameters validParams<VolumeNearestNodeDistanceAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredParam<BoundaryName>("paired_boundary", "The boundary to find the distance to.");
  params.set<bool>("use_displaced_mesh") = true;
  return params;
}

VolumeNearestNodeDistanceAux::VolumeNearestNodeDistanceAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters)/*,
    _nearest_node(_nodal ? getVolumeNearestNodeLocator(, subdomainNames()[0]) :
                           getQuadratureVolumeNearestNodeLocator(parameters.get<BoundaryName>("paired_boundary"), ))*/
{
  if (blocks().size() > 1)
    mooseError("VolumeNearestNodeDistanceAux can only be used with one block at a time!");

  unsigned int boundary_id = _mesh.getBoundaryID(parameters.get<BoundaryName>("paired_boundary"));
  unsigned int block_id  = _mesh.getSubdomainID(blocks()[0]);
  _subproblem.addGhostedBoundary(boundary_id);
  //_subproblem.addGhostedBoundary(block_id);

  //getVolumeNearestNodeLocator
  _nearest_node = new VolumeNearestNodeLocator(_subproblem, _mesh, boundary_id, block_id);
  /*  _nearest_node_locators[std::pair<unsigned int, unsigned int>(master_id, slave_id)] = nnl;
  //getQuadratureVolumeNearestNodeLocator
  unsigned int base_id = 1e6;
  unsigned int qslave_id = slave_id + base_id;
  _slave_to_qslave[slave_id] = qslave_id;
  return getNearestNodeLocator(master_id, qslave_id);
  */

  _nearest_node->findNodes();
}

VolumeNearestNodeDistanceAux::~VolumeNearestNodeDistanceAux()
{
  delete _nearest_node;
  _nearest_node = NULL;
}

Real
VolumeNearestNodeDistanceAux::computeValue()
{

  if (_nodal)
    return _nearest_node->distance(_current_node->id());

  Node * qnode = _mesh.getQuadratureNode(_current_elem, _current_side, _qp);

  return _nearest_node->distance(qnode->id());
}
