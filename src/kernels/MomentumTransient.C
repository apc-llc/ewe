/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "MomentumTransient.h"

#include "Material.h"

template<>
InputParameters validParams<MomentumTransient>()
{
  InputParameters params = validParams<TimeDerivative>();
  params.addParam<Real>("rho", "Mass density");
  params.addCoupledVar("v",  "velocity");
  return params;
}

MomentumTransient::MomentumTransient(const std::string & name,
                                             InputParameters parameters) :
    TimeDerivative(name,parameters),
    _rho(getParam<Real>("rho"))
{}

Real
MomentumTransient::computeQpResidual()
{
  return _rho*_test[_i][_qp]*coupledDot("v")[_qp];
}

Real
MomentumTransient::computeQpJacobian()
{
  return _rho*_test[_i][_qp]*_phi[_j][_qp]*coupledDotDu("v")[_qp];
}
