#include "CardiacMaterialVolumeRatio.h"

template<>
InputParameters validParams<CardiacMaterialVolumeRatioPostprocessor>()
{
  InputParameters params = validParams<ElementIntegralPostprocessor>();
  return params;
}

CardiacMaterialVolumeRatioPostprocessor::CardiacMaterialVolumeRatioPostprocessor(const std::string & name, InputParameters parameters) :
    ElementIntegralPostprocessor(name, parameters),
    _J(getMaterialProperty<Real>("det_displacement_gradient"))
{}

Real
CardiacMaterialVolumeRatioPostprocessor::computeQpIntegral()
{
  return _J[_qp];
}
