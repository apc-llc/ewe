/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "SecondOrderImplicitEulerWithDensity.h"

#include "Material.h"

template<>
InputParameters validParams<SecondOrderImplicitEulerWithDensity>()
{
  InputParameters params = validParams<SecondDerivativeImplicitEuler>();
  params.addParam<Real>("density", 1.0, "Mass density");
  params.addParam<bool>("lumping", false, "True for mass matrix lumping, false otherwise");
  return params;
}

SecondOrderImplicitEulerWithDensity::SecondOrderImplicitEulerWithDensity(const std::string & name,
                                             InputParameters parameters) :
    SecondDerivativeImplicitEuler(name,parameters),
    _density(getParam<Real>("density")),
    _lumping(getParam<bool>("lumping"))
{}

Real
SecondOrderImplicitEulerWithDensity::computeQpResidual()
{
  return -_density*SecondDerivativeImplicitEuler::computeQpResidual();
}

Real
SecondOrderImplicitEulerWithDensity::computeQpJacobian()
{
  return -_density*SecondDerivativeImplicitEuler::computeQpJacobian();
}

void
SecondOrderImplicitEulerWithDensity::computeJacobian()
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
    SecondDerivativeImplicitEuler::computeJacobian();
}
