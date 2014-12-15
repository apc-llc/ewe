#include "CardiacVolumeRatio.h"

template<>
InputParameters validParams<CardiacVolumeRatioPostprocessor>()
{
  InputParameters params = validParams<CardiacVolumeRatioBase>();
  params.addRequiredCoupledVar("displacements", "The x, y, and z displacement");
  return params;
}

CardiacVolumeRatioPostprocessor::CardiacVolumeRatioPostprocessor(const std::string & name, InputParameters parameters) :
    CardiacVolumeRatioBase(name, parameters)
{
  // see http://mooseframework.org/wiki/Faq/#coupling-to-an-arbitrary-number-of-variables-back-to-top for details on this magic
  _grad_disp.resize(coupledComponents("displacements"));

  mooseAssert(_grad_disp.size() == 3, "CardiacMechanicsMaterial: displacements must have exactly 3 components");

  for (unsigned int i=0; i<_grad_disp.size(); ++i)
    _grad_disp[i] = &coupledGradient("displacements", i);
}

Real
CardiacVolumeRatioPostprocessor::computeQpIntegral()
{
  RealTensorValue F((*_grad_disp[0])[_qp](0) + 1, (*_grad_disp[0])[_qp](1),     (*_grad_disp[0])[_qp](2),
                    (*_grad_disp[1])[_qp](0),     (*_grad_disp[1])[_qp](1) + 1, (*_grad_disp[1])[_qp](2),
                    (*_grad_disp[2])[_qp](0),     (*_grad_disp[2])[_qp](1),     (*_grad_disp[2])[_qp](2) + 1);
  return F.det();
}
