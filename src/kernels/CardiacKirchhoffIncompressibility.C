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
   _F(getMaterialProperty<RealTensorValue>("displacement_gradient")),
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
  unsigned int a, b;

  if (jvar == _xdisp_var) {
    a=1; b=2;
  } else if (jvar == _ydisp_var) {
    a=2; b=0;
  } else if (jvar == _zdisp_var) {
    a=0; b=1;
  } else return 0.;

  return _grad_phi[_j][_qp] *
    (  RealVectorValue(_F[_qp](a,1)*_F[_qp](b,2), _F[_qp](a,2)*_F[_qp](b,0), _F[_qp](a,0)*_F[_qp](b,1))
     - RealVectorValue(_F[_qp](2,1)*_F[_qp](b,1), _F[_qp](a,0)*_F[_qp](b,2), _F[_qp](a,1)*_F[_qp](b,0)));
}
