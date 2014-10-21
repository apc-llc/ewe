/*************************************
 * Template for this file came from moose/modules/solid_mechanics/src/kernels/StressDivergence.C
 *************************************/

#include "CardiacStressDivergence.h"

#include "Material.h"
#include "SymmElasticityTensor.h"

template<>
InputParameters validParams<CardiacStressDivergence>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<unsigned int>("component", "An integer corresponding to the direction the variable this kernel acts in. (0 for x, 1 for y, 2 for z)");
  params.addRequiredCoupledVar("disp_x", "The x displacement");
  params.addRequiredCoupledVar("disp_y", "The y displacement");
  params.addRequiredCoupledVar("disp_z", "The z displacement");

  params.set<bool>("use_displaced_mesh") = true;

  return params;
}


CardiacStressDivergence::CardiacStressDivergence(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _stress(getMaterialProperty<SymmTensor>("stress")),
   _Jacobian_mult(getMaterialProperty<SymmElasticityTensor>("Jacobian_mult")),
   _component(getParam<unsigned int>("component")),
   _xdisp_var(coupled("disp_x")),
   _ydisp_var(coupled("disp_y")),
   _zdisp_var(coupled("disp_z"))
{}

Real
CardiacStressDivergence::computeQpResidual()
{
  return _stress[_qp].rowDot(_component, _grad_test[_i][_qp]);
}

Real
CardiacStressDivergence::computeQpJacobian()
{
  return _Jacobian_mult[_qp].stiffness( _component, _component, _grad_test[_i][_qp], _grad_phi[_j][_qp] );
}

Real
CardiacStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
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
