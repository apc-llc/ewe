#include "CardiacKirchhoffIncompressibilityPenalty.h"

#include "Material.h"
#include "SymmElasticityTensor.h"

template<>
InputParameters validParams<CardiacKirchhoffIncompressibilityPenalty>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("displacements", "The x, y, and z displacement");
  params.addRequiredParam<unsigned int>("component", "An integer corresponding to the direction the variable this kernel acts in. (0 for x, 1 for y, 2 for z)");
  params.addParam<Real>("gamma", 1.0, "Factor in front of the incompressibility kernel. Default: 1.0");

  return params;
}


CardiacKirchhoffIncompressibilityPenalty::CardiacKirchhoffIncompressibilityPenalty(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _F(getMaterialProperty<RealTensorValue>("displacement_gradient")),
   _J(getMaterialProperty<Real>("det_displacement_gradient")),
   _component(getParam<unsigned int>("component")),
   _gamma(getParam<Real>("gamma"))
{

  // see http://mooseframework.org/wiki/Faq/#coupling-to-an-arbitrary-number-of-variables-back-to-top for details on this magic
  mooseAssert(coupledComponents("displacements") == 3, "CardiacStressDivergence: displacements must have exactly 3 components");

  for (unsigned int i=0; i<coupledComponents("displacements"); ++i)
    _disp_var[i]  = coupled("displacements", i);
}

Real
CardiacKirchhoffIncompressibilityPenalty::computeQpResidual()
{
  return _gamma*_test[_i][_qp]*(_J[_qp] - 1.);
}

Real
CardiacKirchhoffIncompressibilityPenalty::computeQpJacobian()
{
  return JacobianElement(_component);
}

Real
CardiacKirchhoffIncompressibilityPenalty::computeQpOffDiagJacobian(unsigned int jvar)
{
  for (unsigned int i=0;i<3;i++)
    if (jvar == _disp_var[i])
      return JacobianElement(i);

  return 0.;
}

Real
CardiacKirchhoffIncompressibilityPenalty::JacobianElement(unsigned int comp)
{
  unsigned int a, b;

  switch(comp) {
    case (0): a=1; b=2; break;
    case (1): a=2; b=0; break;
    case (2): a=0; b=1; break;
    default: mooseError("CardiacKirchhoffIncompressibilityPenalty::JacobianElements(): invalid comp argument.");
  }

  return _gamma * _test[_i][_qp] * _grad_phi[_j][_qp] *
    (  RealVectorValue(_F[_qp](a,1)*_F[_qp](b,2), _F[_qp](a,2)*_F[_qp](b,0), _F[_qp](a,0)*_F[_qp](b,1))
     - RealVectorValue(_F[_qp](a,2)*_F[_qp](b,1), _F[_qp](a,0)*_F[_qp](b,2), _F[_qp](a,1)*_F[_qp](b,0)));
}
