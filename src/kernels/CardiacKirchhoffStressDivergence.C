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
   _Rf(getMaterialProperty<RealTensorValue>("R_fibre")),
   _component(getParam<unsigned int>("component")),
   _grad_disp_x(coupledGradient("dispx")),
   _grad_disp_y(coupledGradient("dispy")),
   _grad_disp_z(coupledGradient("dispz")),
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

  SymmTensor dE(/* 00 */      _grad_phi[_j][_qp](0)*grad_xi(0),
                /* 11 */      _grad_phi[_j][_qp](1)*grad_xi(1),
                /* 22 */      _grad_phi[_j][_qp](2)*grad_xi(2),
                /* 01 */ 0.5*(_grad_phi[_j][_qp](0)*grad_xi(1) + grad_xi(0)*_grad_phi[_j][_qp](1)),
                /* 12 */ 0.5*(_grad_phi[_j][_qp](1)*grad_xi(2) + grad_xi(1)*_grad_phi[_j][_qp](2)),
                /* 02 */ 0.5*(_grad_phi[_j][_qp](0)*grad_xi(2) + grad_xi(0)*_grad_phi[_j][_qp](2)));

  return _stress_derivative[_qp].doubleLeftSymmDoubleRightContraction(_grad_test[_i][_qp], grad_xi, dE)
    + fullContraction(_stress[_qp], _grad_test[_i][_qp], _grad_phi[_j][_qp]);
}

Real
CardiacKirchhoffStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _xdisp_var || jvar == _ydisp_var || jvar == _zdisp_var) {
    // nonlinear variables are displacements u(i)=x(i)-X(i)
    // However, we do need the deformation gradient here: dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
    RealVectorValue grad_xi(_grad_u[_qp]);
    grad_xi(_component) += 1;

    RealVectorValue grad_xk;

    if (jvar == _xdisp_var) {
      grad_xk = _grad_disp_x[_qp];
      grad_xk(0) += 1;
    } else if (jvar == _ydisp_var) {
      grad_xk = _grad_disp_y[_qp];
      grad_xk(1) += 1;
    } else {
      grad_xk = _grad_disp_z[_qp];
      grad_xk(2) += 1;
    }

    SymmTensor dE(/* 00 */      _grad_phi[_j][_qp](0)*grad_xk(0),
                  /* 11 */      _grad_phi[_j][_qp](1)*grad_xk(1),
                  /* 22 */      _grad_phi[_j][_qp](2)*grad_xk(2),
                  /* 01 */ 0.5*(_grad_phi[_j][_qp](0)*grad_xk(1) + grad_xk(0)*_grad_phi[_j][_qp](1)),
                  /* 12 */ 0.5*(_grad_phi[_j][_qp](1)*grad_xk(2) + grad_xk(1)*_grad_phi[_j][_qp](2)),
                  /* 02 */ 0.5*(_grad_phi[_j][_qp](0)*grad_xk(2) + grad_xk(0)*_grad_phi[_j][_qp](2)));

    return _stress_derivative[_qp].doubleLeftSymmDoubleRightContraction(_grad_test[_i][_qp], grad_xi, dE);
  } else
    return 0;
}
