#include "CardiacThicknessParameterAux.h"

template<>
InputParameters validParams<CardiacThicknessParameterAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("distance_RV_inner", "Aux variable of the distance to the inner wall of the right ventricle.");
  params.addRequiredCoupledVar("distance_LV_inner", "Aux variable of the distance to the inner wall of the left ventricle.");
  params.addRequiredCoupledVar("distance_outer", "Aux variable of the distance to the outer wall of the heart.");
  params.addParam<bool>("distinguishLVRV", false, "If set to true, the RV will be marked with negative values for e, the LV with positive values. Otherwise, e is always non-negative (default).");
  return params;
}

CardiacThicknessParameterAux::CardiacThicknessParameterAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
    _d_rv(coupledValue("distance_RV_inner")),
    _d_lv(coupledValue("distance_LV_inner")),
    _d_o(coupledValue("distance_outer")),
    _rvfac(getParam<bool>("distinguishLVRV") ? -1 : 1)
{}


Real
CardiacThicknessParameterAux::computeValue()
{
  //TODO: maybe a cleaner regularization instead of adding 1.e-16 to the denominator might make sense...
  if (_d_rv[_qp] < _d_lv[_qp]) {
    // we presumably are in the right ventricle - denote this with a negative thickness parameter
    return _rvfac * _d_o[_qp] / ( _d_o[_qp] + _d_rv[_qp] + 1.e-16 );
  } else {
    // left ventricle --> positive thickness parameter
    return   _d_o[_qp] / ( _d_o[_qp] + _d_lv[_qp] + 1.e-16 );
  }
}
