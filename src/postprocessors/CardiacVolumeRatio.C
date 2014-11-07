#include "CardiacVolumeRatio.h"

template<>
InputParameters validParams<CardiacVolumeRatioPostprocessor>()
{
  InputParameters params = validParams<ElementIntegralPostprocessor>();
  params.addRequiredCoupledVar("dispx", "Displacement in x direction.");
  params.addRequiredCoupledVar("dispy", "Displacement in y direction.");
  params.addRequiredCoupledVar("dispz", "Displacement in z direction.");
  return params;
}

CardiacVolumeRatioPostprocessor::CardiacVolumeRatioPostprocessor(const std::string & name, InputParameters parameters) :
    ElementIntegralPostprocessor(name, parameters),
    _grad_dispx(coupledGradient("dispx")),
    _grad_dispy(coupledGradient("dispy")),
    _grad_dispz(coupledGradient("dispz"))
{}

Real
CardiacVolumeRatioPostprocessor::computeQpIntegral()
{
  RealTensorValue F(_grad_dispx[_qp](0) + 1, _grad_dispx[_qp](1),     _grad_dispx[_qp](2),
                    _grad_dispy[_qp](0),     _grad_dispy[_qp](1) + 1, _grad_dispy[_qp](2),
                    _grad_dispz[_qp](0),     _grad_dispz[_qp](1),     _grad_dispz[_qp](2) + 1);
  return F.det();
}
