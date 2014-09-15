/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "SecondDerivativeNewmark.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<SecondDerivativeNewmark>()
{
  InputParameters params = validParams<TimeKernel>();
  params.addParam<Real>("density", 1.0, "Mass density");
  params.addParam<bool>("lumping", false, "True for mass matrix lumping, false otherwise");
  //params.set<bool>("use_displaced_mesh") = true;

  return params;
}

SecondDerivativeNewmark::SecondDerivativeNewmark(const std::string & name, InputParameters parameters) :
    TimeKernel(name, parameters),
    _comp_name(variable().name()),  // we use the name that was supplied as disp parameter to ensure unique naming and finally correct correspondence to NewmarkMaterial.C
    _density(getParam<Real>("density")),
    _lumping(getParam<bool>("lumping")),
    _acc(getMaterialPropertyOld<Real>("newmark_acceleration-" + _comp_name)),
    _vel(getMaterialPropertyOld<Real>("newmark_velocity-"     + _comp_name)),
    _gamma(getMaterialProperty<Real>( "newmark_gamma-"        + _comp_name)),
    _beta( getMaterialProperty<Real>( "newmark_beta-"         + _comp_name))
{}

Real
SecondDerivativeNewmark::computeQpResidual()
{
  Real accel = ( _u[_qp] / (_dt*_dt) + _vel[_qp] / (_dt) + (0.5-_beta[_qp])*_acc[_qp] ) / _beta[_qp];
  return -_density * _test[_i][_qp] * accel;
}

Real
SecondDerivativeNewmark::computeQpJacobian()
{
  return -_density * _test[_i][_qp] * _phi[_j][_qp] * 1./(_beta[_qp]*_dt*_dt);
}

void
SecondDerivativeNewmark::computeJacobian()
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
