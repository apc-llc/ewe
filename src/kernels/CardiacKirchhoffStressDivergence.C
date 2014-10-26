/*************************************
 * Template for this file came from moose/modules/solid_mechanics/src/kernels/StressDivergence.C
 *************************************/

#include "CardiacKirchhoffStressDivergence.h"

#include "Material.h"
#include "SymmElasticityTensor.h"

template<>
InputParameters validParams<CardiacKirchhoffStressDivergence>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<unsigned int>("component", "An integer corresponding to the direction the variable this kernel acts in. (0 for x, 1 for y, 2 for z)");
  params.addRequiredCoupledVar("disp_x", "The x displacement");
  params.addRequiredCoupledVar("disp_y", "The y displacement");
  params.addRequiredCoupledVar("disp_z", "The z displacement");

  params.set<bool>("use_displaced_mesh") = true;

  return params;
}


CardiacKirchhoffStressDivergence::CardiacKirchhoffStressDivergence(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _stress(getMaterialProperty<RealTensorValue>("Kirchhoff_stress")),
   _stress_derivative(getMaterialProperty<SymmGenericElasticityTensor>("Kirchhoff_stress_derivative")),
   _component(getParam<unsigned int>("component")),
   _xdisp_var(coupled("disp_x")),
   _ydisp_var(coupled("disp_y")),
   _zdisp_var(coupled("disp_z"))
{}

Real
CardiacKirchhoffStressDivergence::computeQpResidual()
{
  // displacement gradient vector du(i)/dX(M) where i==_component
  RealVectorValue grad_xi(_grad_u[_qp]);
  // make a deformation gradient vector from this
  grad_xi(_component) += 1.;

  Real res(0);
  const RealVectorValue grad_test(_grad_test[_i][_qp]);
  // compute _grad_test[_i][_qp] * _stress[_qp] * grad_xi
  for (unsigned int M=0;M<3;M++)
    for (unsigned int N=0;N<3;N++)
      res += grad_test(M)*_stress[_qp](M,N)*grad_xi(N);

  return res;
}

/* TODO
Real
CardiacKirchhoffStressDivergence::computeQpJacobian()
{
  return _Jacobian_mult[_qp].stiffness( _component, _component, _grad_test[_i][_qp], _grad_phi[_j][_qp] );
}

Real
CardiacKirchhoffStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
{
  unsigned int coupled_component = 0;

  bool active(false);

  if (jvar == _xdisp_var )
  {
    coupled_component = 0;
    active = true;
  }
  else if (jvar == _ydisp_var )
  {
    coupled_component = 1;
    active = true;
  }
  else if (jvar == _zdisp_var )
  {
    coupled_component = 2;
    active = true;
  }

  if ( active )
  {
    return _Jacobian_mult[_qp].stiffness( _component, coupled_component,
                                          _grad_test[_i][_qp], _grad_phi[_j][_qp] );
  }

  return 0;
}
*/