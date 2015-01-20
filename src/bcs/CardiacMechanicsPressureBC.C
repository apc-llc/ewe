#include "CardiacMechanicsPressureBC.h"
#include "Function.h"
#include "MooseError.h"

template<>
InputParameters validParams<CardiacMechanicsPressureBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<int>("component", "The component for the pressure, i.e. the index of the displacement component this BC works on.");
  params.addParam<Real>("value", 0.0, "The factor to use in computing the pressure");
  params.addParam<FunctionName>("function", "The function that describes the pressure");
  params.addParam<bool>("use_current_configuration", false, "If set to true, the pressure is prescribed in the current (deformed) configuration, otherwise in the reference configuration.");
  params.set<bool>("use_displaced_mesh") = false;
  return params;
}

CardiacMechanicsPressureBC::CardiacMechanicsPressureBC(const std::string & name, InputParameters parameters)
  :IntegratedBC(name, parameters),
   _component(getParam<int>("component")),
   _value(getParam<Real>("value")),
   _function( isParamValid("function") ? &getFunction("function") : NULL ),
   _current_config(getParam<bool>("use_current_configuration")),
   _J(getMaterialProperty<Real>("det_displacement_gradient")),
   _Finv(getMaterialProperty<RealTensorValue>("inv_displacement_gradient"))
{

  if (_component < 0 || _component > 2)
    mooseError( "CardiacMechanicsPressureBC: Invalid component given for " << name << ": " << _component << "." << std::endl);
}

Real
CardiacMechanicsPressureBC::computeQpResidual()
{
  Real pressure(_value);

  if (_function)
    pressure *= _function->value(_t, _q_point[_qp]);

  if (_current_config)
    return - _test[_i][_qp] * pressure * _J[_qp] * (_normals[_qp](0)*_Finv[_qp](0,_component)
                                                   +_normals[_qp](1)*_Finv[_qp](1,_component)
                                                   +_normals[_qp](2)*_Finv[_qp](2,_component));
  else
    return - _test[_i][_qp] * pressure * _normals[_qp](_component);
}
