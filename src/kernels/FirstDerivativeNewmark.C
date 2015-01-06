#include "FirstDerivativeNewmark.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<FirstDerivativeNewmark>()
{
  InputParameters params = validParams<TimeKernel>();
  params.addParam<Real>("friction", 1.0, "Friction coefficient");
  params.addParam<bool>("lumping", false, "True for mass matrix lumping, false otherwise");
  //params.set<bool>("use_displaced_mesh") = true;

  return params;
}

FirstDerivativeNewmark::FirstDerivativeNewmark(const std::string & name, InputParameters parameters) :
    TimeKernel(name, parameters),
    _comp_name(variable().name()),  // we use the name that was supplied as disp parameter to ensure unique naming and finally correctly correspondence to NewmarkMaterial.C
    _friction(getParam<Real>("friction")),
    _lumping(getParam<bool>("lumping")),
    _acc(getMaterialPropertyOld<Real>("newmark_acceleration-" + _comp_name)),
    _vel(getMaterialPropertyOld<Real>("newmark_velocity-"     + _comp_name)),
    _gamma(getMaterialProperty<Real>( "newmark_gamma-"        + _comp_name)),
    _beta( getMaterialProperty<Real>( "newmark_beta-"         + _comp_name))
{}

Real
FirstDerivativeNewmark::computeQpResidual()
{
  Real gob = _gamma[_qp]/_beta[_qp];
  Real vel = gob*_u[_qp]/_dt + (gob-1.)*_vel[_qp] + _dt/2.*(gob-2.)*_acc[_qp];
  return _friction * _test[_i][_qp] * vel;
}

Real
FirstDerivativeNewmark::computeQpJacobian()
{
  return _friction * _test[_i][_qp] * _phi[_j][_qp] * _gamma[_qp]/(_beta[_qp]*_dt);
}

void
FirstDerivativeNewmark::computeJacobian()
{
  // Idea of mass matrix lumping taken from TimeDerivative.C
  if (_lumping)
  {
    DenseMatrix<Number> & ke = _assembly.jacobianBlock(_var.number(), _var.number());

    for (_i = 0; _i < _test.size(); _i++)
      for (_j = 0; _j < _phi.size(); _j++)
        for (_qp = 0; _qp < _qrule->n_points(); _qp++)
        {
          ke(_i, _i) += _JxW[_qp] * _coord[_qp] * computeQpJacobian();
        }
  }
  else
    TimeKernel::computeJacobian();
}
