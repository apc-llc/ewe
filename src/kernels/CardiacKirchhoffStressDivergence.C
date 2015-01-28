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
   _stress_derivative(getMaterialProperty<CardiacElasticityTensor>("Kirchhoff_stress_derivative")),
   _Cinv(getMaterialProperty<SymmTensor>("Cinv")),
   _J(getMaterialProperty<Real>("det_displacement_gradient")),
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
CardiacKirchhoffStressDivergence::JacobianSecondOrderContribution(const unsigned int i, const unsigned int k)
{
  // nonlinear variables are displacements u(i)=x(i)-X(i)
  // However, we do need the deformation gradient here: dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
  RealVectorValue grad_xi( (*_grad_disp[i])[_qp] );
  grad_xi(i) += 1;

  RealVectorValue grad_xk( (*_grad_disp[k])[_qp] );
  grad_xk(k) += 1;


  const SymmTensor dE(/* 00 */      _grad_phi[_j][_qp](0)*grad_xk(0),
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
  return _grad_phi[_j][_qp]*(_stress[_qp]*_grad_test[_i][_qp])
    + JacobianSecondOrderContribution(_component, _component);
}

Real
CardiacKirchhoffStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (_has_p && jvar == _p_var) {
    // nonlinear variables are displacements u(i)=x(i)-X(i)
    // However, we do need the deformation gradient here: dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
    RealVectorValue grad_xi(_grad_u[_qp]);
    grad_xi(_component) += 1;

    return -_J[_qp]*_phi[_j][_qp]*grad_xi*(_Cinv[_qp]*_grad_test[_i][_qp]);
  }

  // d R^[component k] / d_disp_j
  if (jvar == _disp_var[0]) return JacobianSecondOrderContribution(_component, 0);
  if (jvar == _disp_var[1]) return JacobianSecondOrderContribution(_component, 1);
  if (jvar == _disp_var[2]) return JacobianSecondOrderContribution(_component, 2);

  return 0;
}
