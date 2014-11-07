#include "CardiacKirchhoffStressDivergence.h"

#include "Material.h"
#include "SymmElasticityTensor.h"

template<>
InputParameters validParams<CardiacKirchhoffStressDivergence>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<unsigned int>("component", "An integer corresponding to the direction the variable this kernel acts in. (0 for x, 1 for y, 2 for z)");
  params.addRequiredCoupledVar("dispx", "The x displacement");
  params.addRequiredCoupledVar("dispy", "The y displacement");
  params.addRequiredCoupledVar("dispz", "The z displacement");

  return params;
}


CardiacKirchhoffStressDivergence::CardiacKirchhoffStressDivergence(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _stress(getMaterialProperty<RealTensorValue>("Kirchhoff_stress")),
   _stress_derivative(getMaterialProperty<SymmGenericElasticityTensor>("Kirchhoff_stress_derivative")),
   _component(getParam<unsigned int>("component")),
   _xdisp_var(coupled("dispx")),
   _ydisp_var(coupled("dispy")),
   _zdisp_var(coupled("dispz"))
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
  // nonlinear variables are displacements u(i)=x(i)-X(i)
  // However, we do need the deformation gradient here: dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
  RealVectorValue grad_xi(_grad_u[_qp]);
  grad_xi(_component) += 1;

  return fullContraction(_stress[_qp], _grad_test[_i][_qp], _grad_phi[_j][_qp])
    + _stress_derivative[_qp].doubleLeftSymmDoubleRightContraction(_grad_test[_i][_qp], grad_xi,
                                                                   _grad_phi[_j][_qp], grad_xi );
}

Real
CardiacKirchhoffStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
{
  mooseAssert( ~( jvar == _xdisp_var && _component==0
               || jvar == _ydisp_var && _component==1
               || jvar == _zdisp_var && _component==2), "CardiacKirchhoffStressDivergence::computeQpOffDiagJacobian() called for a diagonal element. Presumably, _component is wrong here.");

  if (jvar == _xdisp_var || jvar == _ydisp_var || jvar == _zdisp_var) {
    // nonlinear variables are displacements u(i)=x(i)-X(i)
    // However, we do need the deformation gradient here: dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
    RealVectorValue grad_xi(_grad_u[_qp]);
    grad_xi(_component) += 1;

    return _stress_derivative[_qp].doubleLeftSymmDoubleRightContraction(_grad_test[_i][_qp], grad_xi,
                                                                        _grad_phi[_j][_qp], grad_xi );
  } else
    return 0;
}
