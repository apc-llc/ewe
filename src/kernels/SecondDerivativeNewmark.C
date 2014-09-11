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
  MooseEnum component("X=0, Y=1, Z=2");
  params.addRequiredParam<MooseEnum>("component", component, "Component, this kernel works on: X, Y, or Z.");
  return params;
}

SecondDerivativeNewmark::SecondDerivativeNewmark(const std::string & name, InputParameters parameters) :
    TimeKernel(name, parameters),
    _density(getParam<Real>("density")),
    _lumping(getParam<bool>("lumping")),
    _c(getParam<MooseEnum>("component")),
    _acc(getMaterialProperty<Point>("newmark_acceleration")),
    _jacobian(getMaterialProperty<Point>("newmark_jacobian"))
{}

Real
SecondDerivativeNewmark::computeQpResidual()
{
  return _density * _test[_i][_qp] * _acc[_qp](_c);
}

Real
SecondDerivativeNewmark::computeQpJacobian()
{
  return _test[_i][_qp] * _phi[_j][_qp] * _jacobian[_qp](_c);
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
