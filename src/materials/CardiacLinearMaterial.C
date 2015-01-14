#include "CardiacLinearMaterial.h"
#include "TensorHelpers.h"

using namespace TensorHelpers;

template<>
InputParameters validParams<CardiacLinearMaterial>()
{
  InputParameters params = validParams<CardiacMechanicsMaterial>();
  params.addRequiredParam<Real>("lambda", "Lame constant.");
  params.addRequiredParam<Real>("mu", "Lame constant.");
  return params;
}

CardiacLinearMaterial::CardiacLinearMaterial(const std::string  & name,
                                                 InputParameters parameters)
  :CardiacMechanicsMaterial(name, parameters),
   _lambda(getParam<Real>("lambda")),
   _mu(getParam<Real>("mu"))
{
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

