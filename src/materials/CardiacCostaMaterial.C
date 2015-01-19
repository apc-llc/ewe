#include "CardiacCostaMaterial.h"
#include "TensorHelpers.h"
#include <limits>

using namespace TensorHelpers;

template<>
InputParameters validParams<CardiacCostaMaterial>()
{
  InputParameters params = validParams<CardiacMechanicsMaterial>();
  params.addRequiredParam<std::vector<Real> >("material_parameters", "Material parameters in following order: C, b_f, b_t, b_fs. The C-parameter has to be given in kPa, any b is unitless.");
  params.addParam<Real>("maximumQ", std::numeric_limits<Real>::max(), "Maximum value for Q to be allowed (to prevent overflow when computing exp(Q).");
  return params;
}

CardiacCostaMaterial::CardiacCostaMaterial(const std::string  & name,
                                                 InputParameters parameters)
  :CardiacMechanicsMaterial(name, parameters),
   _p(getParam<std::vector<Real> >("material_parameters")),
   _maxQ(getParam<Real>("maximumQ")),
   _b(_p[bf], _p[bt], _p[bt], _p[bfs], _p[bt], _p[bfs])
{
  if (_p.size()!=4)
    mooseError("CardiacCostaMaterial: invalid number of entries in material_parameters.");
}

void
CardiacCostaMaterial::computeQpStressProperties(const SymmTensor & /*C*/, const SymmTensor & E)
{
  const Real Q( std::min(_maxQ,
                _p[bf ]* E(0,0)*E(0,0)
               +_p[bt ]*(E(1,1)*E(1,1) + E(2,2)*E(2,2) + E(1,2)*E(1,2) + E(2,1)*E(2,1))
               +_p[bfs]*(E(0,1)*E(0,1) + E(1,0)*E(1,0) + E(0,2)*E(0,2) + E(2,0)*E(2,0)) ) );
  const SymmTensor bE(elementwiseProduct(_b, E));
  const Real CExpQ( _p[C]*std::exp(Q) );

  // elastic energy contribution
  _W[_qp] = (CExpQ - _p[C])/2.;

  // stress
  _stress[_qp] = bE * CExpQ;

  // stress derivative
  for (int M=0;M<3;M++)
    for (int N=M;N<3;N++)
      for (int P=0;P<3;P++)
        for (int Q=P;Q<3;Q++)
          _stress_derivative[_qp](M,N,P,Q) = CExpQ * ( 2.*bE(M,N)*bE(P,Q) + _b(M,N)*_id(M,P)*_id(N,Q) );
}

