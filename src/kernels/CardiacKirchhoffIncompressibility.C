#include "CardiacKirchhoffIncompressibility.h"

#include "Material.h"
#include "SymmElasticityTensor.h"

template<>
InputParameters validParams<CardiacKirchhoffIncompressibility>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("dispx", "The x displacement");
  params.addRequiredCoupledVar("dispy", "The y displacement");
  params.addRequiredCoupledVar("dispz", "The z displacement");

  return params;
}


CardiacKirchhoffIncompressibility::CardiacKirchhoffIncompressibility(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _J(getMaterialProperty<Real>("det_displacement_gradient")),
   _xdisp_var(coupled("dispx")),
   _ydisp_var(coupled("dispy")),
   _zdisp_var(coupled("dispz"))
{}

Real
CardiacKirchhoffIncompressibility::computeQpResidual()
{
  return _test[_i][_qp]*(_J[_qp] - 1.);
}

Real
CardiacKirchhoffIncompressibility::computeQpJacobian()
{
  return 0;
}

Real
CardiacKirchhoffIncompressibility::computeQpOffDiagJacobian(unsigned int jvar)
{
/*
  mooseAssert( ~( jvar == _xdisp_var && _component==0
               || jvar == _ydisp_var && _component==1
               || jvar == _zdisp_var && _component==2), "CardiacKirchhoffIncompressibility::computeQpOffDiagJacobian() called for a diagonal element. Presumably, _component is wrong here.");

  if (jvar == _xdisp_var || jvar == _ydisp_var || jvar == _zdisp_var) {
    // nonlinear variables are displacements u(i)=x(i)-X(i)
    // However, we do need the deformation gradient here: dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
    RealVectorValue grad_xi(_grad_u[_qp]);
    grad_xi(_component) += 1;

    return _stress_derivative[_qp].doubleLeftSymmDoubleRightContraction(_grad_test[_i][_qp], grad_xi,
                                                                        _grad_phi[_j][_qp], grad_xi );
  } else
*/    return 0;
}
