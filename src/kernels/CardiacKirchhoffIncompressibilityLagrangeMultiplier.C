#include "CardiacKirchhoffIncompressibilityLagrangeMultiplier.h"

#include "Material.h"
#include "SymmElasticityTensor.h"

template<>
InputParameters validParams<CardiacKirchhoffIncompressibilityLagrangeMultiplier>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("displacements", "The x, y, and z displacement");

  return params;
}


CardiacKirchhoffIncompressibilityLagrangeMultiplier::CardiacKirchhoffIncompressibilityLagrangeMultiplier(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _F(getMaterialProperty<RealTensorValue>("displacement_gradient")),
   _invF(getMaterialProperty<RealTensorValue>("inv_displacement_gradient")),
   _J(getMaterialProperty<Real>("det_displacement_gradient"))
{

  // see http://mooseframework.org/wiki/Faq/#coupling-to-an-arbitrary-number-of-variables-back-to-top for details on this magic
  mooseAssert(coupledComponents("displacements") == 3, "CardiacStressDivergence: displacements must have exactly 3 components");

  for (unsigned int i=0; i<coupledComponents("displacements"); ++i)
    _disp_var[i]  = coupled("displacements", i);
}

Real
CardiacKirchhoffIncompressibilityLagrangeMultiplier::computeQpResidual()
{
  return _test[_i][_qp]*(_J[_qp] - 1.);
}

Real
CardiacKirchhoffIncompressibilityLagrangeMultiplier::computeQpJacobian()
{
  return 0;
}

Real
CardiacKirchhoffIncompressibilityLagrangeMultiplier::computeQpOffDiagJacobian(unsigned int jvar)
{
  for (unsigned int i=0;i<3;i++)
    if (jvar == _disp_var[i]) {
      return _test[_i][_qp]*_J[_qp]*(_grad_phi[_j][_qp](0)*_invF[_qp](0,i)
                                    +_grad_phi[_j][_qp](1)*_invF[_qp](1,i)
                                    +_grad_phi[_j][_qp](2)*_invF[_qp](2,i));
    }

  return 0.;
}

