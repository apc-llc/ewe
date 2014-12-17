#include "CardiacVolumeRatioBase.h"

template<>
InputParameters validParams<CardiacVolumeRatioBase>()
{
  InputParameters params = validParams<ElementIntegralPostprocessor>();
  return params;
}

CardiacVolumeRatioBase::CardiacVolumeRatioBase(const std::string & name, InputParameters parameters) :
    ElementIntegralPostprocessor(name, parameters),
    _volume(0)
{}

void
CardiacVolumeRatioBase::initialize()
{
  ElementIntegralPostprocessor::initialize();
  _volume = 0;
}

void
CardiacVolumeRatioBase::execute()
{
  ElementIntegralPostprocessor::execute();
  _volume += _current_elem_volume;
}

Real
CardiacVolumeRatioBase::getValue()
{
  Real integral = ElementIntegralPostprocessor::getValue();
  gatherSum(_volume);
  return integral / _volume;
}

void
CardiacVolumeRatioBase::threadJoin(const UserObject & y)
{
  ElementIntegralPostprocessor::threadJoin(y);
  const CardiacVolumeRatioBase & pps = static_cast<const CardiacVolumeRatioBase &>(y);
  _volume += pps._volume;
}

