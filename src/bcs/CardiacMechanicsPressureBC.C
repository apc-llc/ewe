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
  params.addRequiredCoupledVar("displacements", "The x, y, and z displacement");
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

  // see http://mooseframework.org/wiki/Faq/#coupling-to-an-arbitrary-number-of-variables-back-to-top for details on this magic
  mooseAssert(coupledComponents("displacements") == 3, "CardiacKirchhoffStressDivergence: displacements must have exactly 3 components");

  for (unsigned int i=0; i<3; ++i) {
    _disp_var[i]  = coupled("displacements", i);
  }
}

Real
CardiacMechanicsPressureBC::computeQpResidual()
{
  Real pressure(_value);

  if (_function)
    pressure *= _function->value(_t, _q_point[_qp]);

  if (_current_config) {
    Real n_comp(_normals[_qp](0)*_Finv[_qp](0,_component)
               +_normals[_qp](1)*_Finv[_qp](1,_component)
               +_normals[_qp](2)*_Finv[_qp](2,_component));
    return _test[_i][_qp] * pressure * _J[_qp] * n_comp;
  } else
    return _test[_i][_qp] * pressure * _normals[_qp](_component);
}


Real
CardiacMechanicsPressureBC::computeQpJacobian()
{
  return JacobianEntry(_component, _component);
}


Real
CardiacMechanicsPressureBC::computeQpOffDiagJacobian(unsigned int jvar)
{
  // d R^[component k] / d_disp_j
  if (jvar == _disp_var[0]) return JacobianEntry(_component, 0);
  if (jvar == _disp_var[1]) return JacobianEntry(_component, 1);
  if (jvar == _disp_var[2]) return JacobianEntry(_component, 2);
  return 0.;
}


Real
CardiacMechanicsPressureBC::JacobianEntry(unsigned int i, unsigned int k)
{
  if (_current_config) {
    Real pressure(_value);

    if (_function)
      pressure *= _function->value(_t, _q_point[_qp]);

    RealVectorValue n( _Finv[_qp].transpose() * _normals[_qp] );
    RealVectorValue g( _Finv[_qp].transpose() * _grad_phi[_j][_qp] );

    return _test[_i][_qp] * pressure * _J[_qp] * ( g(k)*n(i) - g(i)*n(k) );
  } else
    return 0.;
}

