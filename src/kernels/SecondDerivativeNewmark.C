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
  params.addParam<Real>("alpha", 0.5, "Newmark integration alpha parameter (default: 0.5 --> yields 2nd order method, 0. --> fully implicit, 1. --> fully explicit)");
  return params;
}

SecondDerivativeNewmark::SecondDerivativeNewmark(const std::string & name, InputParameters parameters) :
    TimeKernel(name, parameters),
    _comp_name(variable().name()),  // we use the name that was supplied as disp parameter to ensure unique naming and finally correctly correspondence to NewmarkMaterial.C
    _density(getParam<Real>("density")),
    _lumping(getParam<bool>("lumping")),
    _alpha(getParam<Real>("alpha")),
    _acc(getMaterialProperty<Real>("newmark_acceleration-" + _comp_name)),
    _acc_old(getMaterialPropertyOld<Real>("newmark_acceleration-" + _comp_name))
{}

Real
SecondDerivativeNewmark::computeQpResidual()
{
  return _density * _test[_i][_qp] * ( (1.-_alpha)*_acc[_qp] + (_alpha)*_acc_old[_qp]);
}

Real
SecondDerivativeNewmark::computeQpJacobian()
{
  return _density * _test[_i][_qp] * _phi[_j][_qp] * (1.-_alpha)/(_dt*_dt);
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
