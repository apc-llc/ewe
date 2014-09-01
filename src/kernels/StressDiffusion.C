/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "StressDiffusion.h"
#include "MooseEnum.h"

/**
 * This function defines the valid parameters for
 * this Kernel and their default values
 */
template<>
InputParameters validParams<StressDiffusion>()
{
  InputParameters params = validParams<Kernel>();
  MooseEnum component("X=0, Y=1, Z=2");
  params.addRequiredParam<MooseEnum>("component", component, "Component, this kernel works on (selects row in 2ndPiolaKirchoff stress tensor): X, Y, or Z.");
  return params;
}


StressDiffusion::StressDiffusion(const std::string & name,
                                   InputParameters parameters) :
    Kernel(name,parameters),
    _T(getMaterialProperty<RealTensorValue>("PiolaKirchoff2nd")),
    _c(getParam<MooseEnum>("component"))
{}

Real
StressDiffusion::computeQpResidual()
{
  return ((_grad_test[_i][_qp])*(_T[_qp].row(_c)*_grad_u[_qp]))(0); // TODO: explicit conversion libMesh::TypeVector<double> to libMesh::Real ; have to check if this is really a 1-entry vector before
}

Real
StressDiffusion::computeQpJacobian()
{
  return ((_grad_test[_i][_qp])*(_T[_qp].row(_c)*_grad_phi[_j][_qp]))(0); // TODO: dito
}
