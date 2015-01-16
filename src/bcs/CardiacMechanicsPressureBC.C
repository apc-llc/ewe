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
  params.set<bool>("use_displaced_mesh") = false;
  return params;
}

CardiacMechanicsPressureBC::CardiacMechanicsPressureBC(const std::string & name, InputParameters parameters)
  :IntegratedBC(name, parameters),
   _component(getParam<int>("component")),
   _value(getParam<Real>("value")),
   _function( isParamValid("function") ? &getFunction("function") : NULL )
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

  return - _test[_i][_qp] * pressure * _normals[_qp](_component);
}
