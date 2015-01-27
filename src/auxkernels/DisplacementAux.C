#include "DisplacementAux.h"

template<>
InputParameters validParams<DisplacementAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredParam<unsigned int>("component", "An integer corresponding to the direction the variable this kernel acts in. (0 for x, 1 for y, 2 for z)");
  params.addRequiredCoupledVar("coordinate", "The displaced coordinate.");
  return params;
}

DisplacementAux::DisplacementAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
    _component(getParam<unsigned int>("component")),
    _coordinate(coupledValue("coordinate"))
{}


Real
DisplacementAux::computeValue()
{
  return _coordinate[_qp] - _q_point[_qp](_component);
}
