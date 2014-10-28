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
  params.addRequiredCoupledVar("x", "The x displaced coordinate");
  params.addRequiredCoupledVar("y", "The y displaced coordinate");
  params.addRequiredCoupledVar("z", "The z displaced coordinate");

  return params;
}


CardiacKirchhoffStressDivergence::CardiacKirchhoffStressDivergence(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _stress(getMaterialProperty<RealTensorValue>("Kirchhoff_stress")),
   _stress_derivative(getMaterialProperty<SymmGenericElasticityTensor>("Kirchhoff_stress_derivative")),
   _component(getParam<unsigned int>("component")),
   _xvar(coupled("x")),
   _yvar(coupled("y")),
   _zvar(coupled("z"))
{}

Real CardiacKirchhoffStressDivergence::fullContraction(const RealTensorValue & t,
                                                       const RealVectorValue & v1,
                                                       const RealVectorValue & v2) const
{
  Real res(0);
  for (unsigned int M=0;M<3;M++)
    for (unsigned int N=0;N<3;N++)
      res += v1(M)*t(M,N)*v2(N);
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
  // nonlinear variables are displacements u(i)=x(i)-X(i)
  // However, we do need the deformation gradient here: dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
  RealVectorValue grad_xi(_grad_u[_qp]);
  grad_xi(_component) += 1;

  // compute _grad_test[_i][_qp] * _stress[_qp] * _grad_xi
  return fullContraction(_stress[_qp], _grad_test[_i][_qp], grad_xi);
}

Real
CardiacKirchhoffStressDivergence::computeQpJacobian()
{
  return fullContraction(_stress[_qp], _grad_test[_i][_qp], _grad_phi[_j][_qp]);
    //+ fullContraction(_stress_derivative[_qp], _grad_test[_i][_qp], _grad_phi[_j][_qp], _grad_phi[_j][_qp], _grad_phi[_j][_qp]);
}

/*
Real
CardiacKirchhoffStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
{
  return 0;
  // all displacements enter into the thing in more or less identical form...
  if (jvar == _xvar || jvar == _yvar || jvar == _zvar)
    return computeQpJacobian();
  else
    return 0;
}
*/