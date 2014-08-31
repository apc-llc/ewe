/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "StressDiffusion.h"

/**
 * This function defines the valid parameters for
 * this Kernel and their default values
 */
template<>
InputParameters validParams<StressDiffusion>()
{
  InputParameters params = validParams<StressDiffusion>();
  MooseEnum component("X=0, Y=1, Z=2");
  params.addREquiredParam<MooseEnum>("component", component, "Component, this kernel works on (selects row in 2ndPiolaKirchoff stress tensor): X, Y, or Z.");
  return params;
}


StressDiffusion::StressDiffusion(const std::string & name,
                                   InputParameters parameters) :
    Kernel(name,parameters),
    _T(getMaterialProperty<RealTensorValue>("PiolaKirchoff2nd")),
    _c(getParam<MooseEnum>("component"));
{}

Real
StressDiffusion::computeQpResidual()
{
  return (_grad_test[_c][_qp])*(_T[_qp].row(i)*_grad_u[_qp]);
}

Real
StressDiffusion::computeQpJacobian()
{
  return (_grad_test[_c][_qp])*(_T[_qp].row(i)*_grad_phi[_j][_qp]);
}
