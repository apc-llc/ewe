/****************************************************************/
/*        derived from NearestNodeDistanceAux.C                 */
/****************************************************************/

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
    AuxKernel(name, parameters),
    _nearest_node(_nodal ? getNearestNodeLocator(parameters.get<BoundaryName>("paired_boundary"), boundaryNames()[0]) : getQuadratureNearestNodeLocator(parameters.get<BoundaryName>("paired_boundary"), boundaryNames()[0]))
{
  if (boundaryNames().size() > 1)
    mooseError("VolumeNearestNodeDistanceAux can only be used with one boundary at a time!");
}

VolumeNearestNodeDistanceAux::~VolumeNearestNodeDistanceAux()
{
}

Real
VolumeNearestNodeDistanceAux::computeValue()
{
  if (_nodal)
    return _nearest_node.distance(_current_node->id());

  Node * qnode = _mesh.getQuadratureNode(_current_elem, _current_side, _qp);

  return _nearest_node.distance(qnode->id());
}
