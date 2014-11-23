#include "NeighborElementAverageAux.h"

template<>
InputParameters validParams<NeighborElementAverageAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("input_variable", "Variable to be averaged.");
  params.addParam<bool>("volume_weighting", false, "If true, the values from neighbouring elements will be weighted by the elements volume (in addition to the explicitly set weights). Default: false");
  params.addParam<Real>("weight_centre", 1.0, "Averaging weight for the current (central) element.");
  params.addParam<Real>("weight_neighbor", 1.0, "Averaging weight for the neighbour elements.");
  return params;
}

NeighborElementAverageAux::NeighborElementAverageAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
  _in(coupledValue("input_variable")),
  _in_varptr(getVar("input_variable", 0)),
  _weight_centre(getParam<Real>("weight_centre")),
  _weight_neighbor(getParam<Real>("weight_neighbor")),
  _volume_weighting(getParam<bool>("volume_weighting"))
{}


Real
NeighborElementAverageAux::computeValue()
{
  Real total_weight(0);
  Real sum(0);

  // contribution of all neighbors
  for (unsigned int s=0;s<_current_elem->n_sides();s++) {
    // if Elem* is NULL, that side is either a boundary or someone forgot to
    // call MeshBase::find_neighbors()
    if (Elem* current_neighbour = _current_elem->neighbor(s)) {
      Real weight(_weight_neighbor);

      if (_volume_weighting)
        weight *= current_neighbour->volume();

      sum += weight * _in_varptr->getElementalValue(current_neighbour);
      total_weight += weight;
    }
  }

  if (total_weight == 0)
    mooseWarning("NeighborElementAverageAux found an element without any neighbours.");

  // contribution of central element
  Real weight(_weight_centre);

  if (_volume_weighting)
    weight *= _current_elem->volume();

  sum += weight*_in[_qp];
  total_weight += weight;

  return sum / total_weight;
}
