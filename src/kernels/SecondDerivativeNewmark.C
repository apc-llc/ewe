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
  params.addRequiredParam<std::string>("str_append", "String that has been appended to material property names (connection to NewmarkMaterial)");
  return params;
}

SecondDerivativeNewmark::SecondDerivativeNewmark(const std::string & name, InputParameters parameters) :
    TimeKernel(name, parameters),
    _density(getParam<Real>("density")),
    _lumping(getParam<bool>("lumping")),
    _acc(getMaterialProperty<Real>("newmark_acceleration" + getParam<std::string>("str_append"))),
    _jacobian(getMaterialProperty<Real>("newmark_jacobian" + getParam<std::string>("str_append")))
{}

Real
SecondDerivativeNewmark::computeQpResidual()
{
  return _density * _test[_i][_qp] * _acc[_qp];
}

Real
SecondDerivativeNewmark::computeQpJacobian()
{
  return _test[_i][_qp] * _phi[_j][_qp] * _jacobian[_qp];
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
