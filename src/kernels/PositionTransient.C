/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "PositionTransient.h"

#include "Material.h"

template<>
InputParameters validParams<PositionTransient>()
{
  InputParameters params = validParams<TimeDerivative>();
  params.addCoupledVar("x",  "position");
  return params;
}

PositionTransient::PositionTransient(const std::string & name,
                                             InputParameters parameters) :
    TimeDerivative(name,parameters)
{}

Real
PositionTransient::computeQpResidual()
{
  return _test[_i][_qp]*(coupledDot("x")[_qp]-_u[_qp]);
}

Real
PositionTransient::computeQpJacobian()
{
  return _test[_i][_qp]*_phi[_j][_qp]*(coupledDotDu("x")[_qp]-1.);
}
