#include "CardiacKirchhoffStressDivergence.h"

#include "Material.h"
#include "SymmElasticityTensor.h"

template<>
InputParameters validParams<CardiacKirchhoffStressDivergence>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<unsigned int>("component", "An integer corresponding to the direction the variable this kernel acts in. (0 for x, 1 for y, 2 for z)");
  params.addRequiredCoupledVar("displacements", "The x, y, and z displacement");
  params.addCoupledVar("p", "The hydrostatic pressure (if using an incompressible material)");
  params.set<bool>("use_displaced_mesh") = false;
  return params;
}


CardiacKirchhoffStressDivergence::CardiacKirchhoffStressDivergence(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _stress(getMaterialProperty<SymmTensor>("Kirchhoff_stress")),
   _stress_derivative(getMaterialProperty<SymmGenericElasticityTensor>("Kirchhoff_stress_derivative")),
   _Cinv(getMaterialProperty<SymmTensor>("Cinv")),
   _component(getParam<unsigned int>("component")),
   _has_p(isCoupled("p")),
   _p_var(_has_p ? coupled("p") : 0)
{
  // see http://mooseframework.org/wiki/Faq/#coupling-to-an-arbitrary-number-of-variables-back-to-top for details on this magic
  _grad_disp.resize(coupledComponents("displacements"));

  mooseAssert(_grad_disp.size() == 3, "CardiacKirchhoffStressDivergence: displacements must have exactly 3 components");

  for (unsigned int i=0; i<_grad_disp.size(); ++i) {
    _grad_disp[i] = &coupledGradient("displacements", i);
    _disp_var[i]  = coupled("displacements", i);
  }
}

Real
CardiacKirchhoffStressDivergence::computeQpResidual()
{
  // nonlinear variables are displacements u(i)=x(i)-X(i)
  // However, we do need the deformation gradient here: dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
  RealVectorValue grad_xi(_grad_u[_qp]);
  grad_xi(_component) += 1;

  // compute _grad_xi * _stress[_qp] * _grad_test[_i][_qp]
  return grad_xi*(_stress[_qp]*_grad_test[_i][_qp]);
}

/// This produces the second term in the notes for the Jacobian
Real
CardiacKirchhoffStressDivergence::JacobianSecondOrderContribution(const RealVectorValue & grad_xi, const RealVectorValue & grad_xk)
{
  SymmTensor dE(/* 00 */      _grad_phi[_j][_qp](0)*grad_xk(0),
                /* 11 */      _grad_phi[_j][_qp](1)*grad_xk(1),
                /* 22 */      _grad_phi[_j][_qp](2)*grad_xk(2),
                /* 01 */ 0.5*(_grad_phi[_j][_qp](0)*grad_xk(1) + grad_xk(0)*_grad_phi[_j][_qp](1)),
                /* 12 */ 0.5*(_grad_phi[_j][_qp](1)*grad_xk(2) + grad_xk(1)*_grad_phi[_j][_qp](2)),
                /* 02 */ 0.5*(_grad_phi[_j][_qp](0)*grad_xk(2) + grad_xk(0)*_grad_phi[_j][_qp](2)));

  return _stress_derivative[_qp].doubleLeftSymmDoubleRightContraction(grad_xi, _grad_test[_i][_qp], dE);
}

Real
CardiacKirchhoffStressDivergence::computeQpJacobian()
{
  // nonlinear variables are displacements u(i)=x(i)-X(i)
  // However, we do need the deformation gradient here: dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
  RealVectorValue grad_xi(_grad_u[_qp]);
  grad_xi(_component) += 1;

  return _grad_phi[_j][_qp]*(_stress[_qp]*_grad_test[_i][_qp])
    + JacobianSecondOrderContribution(grad_xi, grad_xi);
}

Real
CardiacKirchhoffStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
{
  int idx(-1);

  if (_has_p && jvar == _p_var) {
    // nonlinear variables are displacements u(i)=x(i)-X(i)
    // However, we do need the deformation gradient here: dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
    RealVectorValue grad_xi(_grad_u[_qp]);
    grad_xi(_component) += 1;

    return -2.*_phi[_j][_qp]*grad_xi*(_Cinv[_qp]*_grad_test[_i][_qp]);
  }

  if (jvar == _disp_var[0])      { idx=0; }
  else if (jvar == _disp_var[1]) { idx=1; }
  else if (jvar == _disp_var[2]) { idx=2; }
  else return 0;

  // nonlinear variables are displacements u(i)=x(i)-X(i)
  // However, we do need the deformation gradient here: dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
  RealVectorValue grad_xi(_grad_u[_qp]);
  grad_xi(_component) += 1;

  RealVectorValue grad_xk( (*_grad_disp[idx])[_qp] );
  grad_xk(idx) += 1;

  return JacobianSecondOrderContribution(grad_xi, grad_xk);
}
