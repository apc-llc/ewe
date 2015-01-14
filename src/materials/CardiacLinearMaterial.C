#include "CardiacLinearMaterial.h"
#include "TensorHelpers.h"

using namespace TensorHelpers;

template<>
InputParameters validParams<CardiacLinearMaterial>()
{
  InputParameters params = validParams<CardiacMechanicsMaterial>();
  params.addParam<Real>("lambda", "1st Lame constant.");
  params.addParam<Real>("mu", "2nd Lame constant.");
  params.addParam<Real>("E", "Youngs modulus.");
  params.addParam<Real>("nu", "Poissons ratio.");
  return params;
}

CardiacLinearMaterial::CardiacLinearMaterial(const std::string  & name,
                                                 InputParameters parameters)
  :CardiacMechanicsMaterial(name, parameters),
   _lambda(isParamValid("lambda") ? getParam<Real>("lambda") : 0),
   _mu(isParamValid("mu") ? getParam<Real>("mu") : 0),
   _E(isParamValid("E") ? getParam<Real>("E") : 0),
   _nu(isParamValid("nu") ? getParam<Real>("nu") : 0)
{
  bool _has_lambda(isParamValid("lambda"));
  bool _has_mu(isParamValid("mu"));
  bool _has_E(isParamValid("E"));
  bool _has_nu(isParamValid("nu"));

  // see http://en.wikipedia.org/wiki/Lamé_parameters for the conversion formulas
  if (_has_lambda && _has_mu && !_has_E && !_has_nu) {
    _E  = _mu*(3.*_lambda+2.*_mu)/(_lambda+_mu);
    _nu = _lambda/(2.*_lambda+_mu);
  } else if (_has_lambda && !_has_mu && _has_E && !_has_nu) {
    _mu = (_E-3.*_lambda)+std::sqrt((_E-3.*_lambda)*(_E-3.*_lambda)+8.*_lambda*_E)/4.;
    _nu = -(_E+_lambda)+std::sqrt((_E+_lambda)*(_E+_lambda)+8.*_lambda*_lambda)/(4.*_lambda);
  } else if (!_has_lambda && _has_mu && _has_E && !_has_nu) {
    _lambda = _mu*(2.*_mu-_E)/(_E-3.*_mu);
    _nu     = (_E-2.*_mu)/(2.*_mu);
  } else if (_has_lambda && !_has_mu && !_has_E && _has_nu) {
    _mu = _lambda*(1.-2.*_nu)/(2.*_nu);
    _E  = _lambda*(1.+_nu)*(1.-2.*_nu)/_nu;
  } else if (!_has_lambda && _has_mu && !_has_E && _has_nu) {
    _lambda = 2.*_mu*_nu/(1.-2.*_nu);
    _E      = 2.*_mu*(1.+_nu);
  } else if (!_has_lambda && !_has_mu && _has_E && _has_nu) {
    _lambda = _E*_nu/((1.+_nu)*(1.-2.*_nu));
    _mu     = _E/(2.+2.*_nu);
  } else
    mooseError("CardiacLinearMaterial: You have to give exactly two parameters of mu, nu, lambda, and/or E.");

  if (_mu <= 0 || _lambda + 2./3.*_mu <= 0)
    mooseError("CardiacLinearMaterial: Invalid Lame constants."); // see [Batra: Linear Constitutive Relations in Isotropic Finite Elasticity, Journal of Elasticity 51: 243–245, 1998]
}

void
CardiacLinearMaterial::computeQpStressProperties(const SymmTensor & /*C*/, const SymmTensor & E)
{
  // elastic energy contribution
  _W[_qp] =  0.; ///< \todo: add energy

  // stress
  _stress[_qp] = STtoRTV( _id * _lambda*E.trace() + E * 2.*_mu);

  // stress derivative
  for (int M=0;M<3;M++)
    for (int N=M;N<3;N++)
      for (int P=0;P<3;P++)
        for (int Q=P;Q<3;Q++)
          _stress_derivative[_qp](M,N,P,Q) = _lambda*_id(P,Q)*_id(M,N) + 2.*_mu*_id(M,P)*_id(N,Q);
}

