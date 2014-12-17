#include "CardiacMaterialVolumeRatio.h"

template<>
InputParameters validParams<CardiacMaterialVolumeRatioPostprocessor>()
{
  InputParameters params = validParams<CardiacVolumeRatioBase>();
  return params;
}

CardiacMaterialVolumeRatioPostprocessor::CardiacMaterialVolumeRatioPostprocessor(const std::string & name, InputParameters parameters) :
    CardiacVolumeRatioBase(name, parameters),
    _J(getMaterialProperty<Real>("det_displacement_gradient")){}

Real
CardiacMaterialVolumeRatioPostprocessor::computeQpIntegral()
{
  return _J[_qp];
}
