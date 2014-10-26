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

Real CardiacKirchhoffStressDivergence::fullContraction(const RealTensorValue & t,
                                                       const RealVectorValue & v1,
                                                       const RealVectorValue & v2) const
{
  Real res(0);
  for (unsigned int M=0;M<3;M++)
    for (unsigned int N=0;N<3;N++)
      res += t(M,N)*v1(M)*v2(N);
  return res;
}

Real CardiacKirchhoffStressDivergence::fullContraction(const SymmGenericElasticityTensor & t,
                                                       const RealVectorValue & v1,
                                                       const RealVectorValue & v2,
                                                       const RealVectorValue & v3,
                                                       const RealVectorValue & v4) const
{
  Real res(0);
  for (unsigned int M=0;M<3;M++)
    for (unsigned int N=0;N<3;N++)
      for (unsigned int P=0;P<3;P++)
        for (unsigned int Q=0;Q<3;Q++)
          res += t(M,N,P,Q)*v1(M)*v2(N)*v3(P)*v4(Q);
  return res;
}

Real
CardiacKirchhoffStressDivergence::computeQpResidual()
{
  // displacement gradient vector du(i)/dX(M) where i==_component
  RealVectorValue grad_xi(_grad_u[_qp]);
  // make a deformation gradient vector from this
  grad_xi(_component) += 1.;
  // other convenience shorthands
  const RealVectorValue grad_test(_grad_test[_i][_qp]);

  // compute _grad_test[_i][_qp] * _stress[_qp] * grad_xi
  return fullContraction(_stress[_qp], grad_test, grad_xi);
}

Real
CardiacKirchhoffStressDivergence::computeQpJacobian()
{
  // deformation gradient vector du(i)/dX(M) where i==_component
  RealVectorValue grad_xi(_grad_u[_qp]);
  grad_xi(_component) += 1.;
  // other convenience shorthands
  const RealVectorValue grad_test(_grad_test[_i][_qp]);
  const RealVectorValue grad_phi(_grad_phi[_j][_qp]);

  return fullContraction(_stress_derivative[_qp], grad_test, grad_phi, grad_phi, grad_phi)
    + fullContraction(_stress[_qp], grad_test, grad_phi);
}

Real
CardiacKirchhoffStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
{
  // all displacements enter into the thing in more or less identical form...
  if (jvar == _xdisp_var || jvar == _ydisp_var || jvar == _zdisp_var)
    return computeQpJacobian();
  else
    return 0;
}
