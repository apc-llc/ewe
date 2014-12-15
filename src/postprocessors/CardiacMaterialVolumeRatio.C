#include "CardiacMaterialVolumeRatio.h"

template<>
InputParameters validParams<CardiacMaterialVolumeRatioPostprocessor>()
{
  InputParameters params = validParams<ElementIntegralPostprocessor>();
  return params;
}

CardiacMaterialVolumeRatioPostprocessor::CardiacMaterialVolumeRatioPostprocessor(const std::string & name, InputParameters parameters) :
    ElementIntegralPostprocessor(name, parameters),
    _J(getMaterialProperty<Real>("det_displacement_gradient")),
    _volume(0)
{}

void
CardiacMaterialVolumeRatioPostprocessor::initialize()
{
  ElementIntegralPostprocessor::initialize();
  _volume = 0;
}

void
CardiacMaterialVolumeRatioPostprocessor::execute()
{
  ElementIntegralPostprocessor::execute();
  _volume += _current_elem_volume;
}

Real
CardiacMaterialVolumeRatioPostprocessor::getValue()
{
  Real integral = ElementIntegralPostprocessor::getValue();
  gatherSum(_volume);
  return integral / _volume;
}

void
CardiacMaterialVolumeRatioPostprocessor::threadJoin(const UserObject & y)
{
  ElementIntegralPostprocessor::threadJoin(y);
  const CardiacMaterialVolumeRatioPostprocessor & pps = static_cast<const CardiacMaterialVolumeRatioPostprocessor &>(y);
  _volume += pps._volume;
}

Real
CardiacMaterialVolumeRatioPostprocessor::computeQpIntegral()
{
  return _J[_qp];
}
