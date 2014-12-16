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
  params.addRequiredCoupledVar("displacements", "The x, y, and z displacement");

  params.set<bool>("use_displaced_mesh") = true;

  return params;
}


CardiacStressDivergence::CardiacStressDivergence(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _stress(getMaterialProperty<SymmTensor>("stress")),
   _Jacobian_mult(getMaterialProperty<SymmElasticityTensor>("Jacobian_mult")),
   _component(getParam<unsigned int>("component"))
{
  // see http://mooseframework.org/wiki/Faq/#coupling-to-an-arbitrary-number-of-variables-back-to-top for details on this magic
  mooseAssert(coupledComponents("displacements") == 3, "CardiacStressDivergence: displacements must have exactly 3 components");

  for (unsigned int i=0; i<coupledComponents("displacements"); ++i) {
    _disp_var[i]  = coupled("displacements", i);
  }
}

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
  unsigned int coupled_component = -1;

  if (jvar == _disp_var[0] )      { coupled_component = 0; }
  else if (jvar == _disp_var[1] ) { coupled_component = 1; }
  else if (jvar == _disp_var[2] ) { coupled_component = 2; }
  else return 0;

  return _Jacobian_mult[_qp].stiffness( _component, coupled_component,
                                        _grad_test[_i][_qp], _grad_phi[_j][_qp] );
}
