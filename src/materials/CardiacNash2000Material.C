#include "CardiacNash2000Material.h"
#include "TensorHelpers.h"

using namespace TensorHelpers;

template<>
InputParameters validParams<CardiacNash2000Material>()
{
  InputParameters params = validParams<CardiacMechanicsMaterial>();
  params.addRequiredParam<std::vector<Real> >("k_MN", "Material parameters k_MN in following order: k_11, k_22, k_33, k_12, k_23, k_13. Unit: kPa.");
  params.addRequiredParam<std::vector<Real> >("a_MN", "Material parameters a_MN in following order: a_11, a_22, a_33, a_12, a_23, a_13. Unitless.");
  params.addRequiredParam<std::vector<Real> >("b_MN", "Material parameters b_MN in following order: b_11, b_22, b_33, b_12, b_23, b_13. Unitless.");
  return params;
}

CardiacNash2000Material::CardiacNash2000Material(const std::string  & name,
                                                 InputParameters parameters)
  :CardiacMechanicsMaterial(name, parameters),
   _k(SymmTensor(getParam<std::vector<Real> >("k_MN"))),
   _a(SymmTensor(getParam<std::vector<Real> >("a_MN"))),
   _b(SymmTensor(getParam<std::vector<Real> >("b_MN")))
{
  for (int M=0;M<3;M++)
    for (int N=M;N<3;N++)
       if (_k(M,N)<0 || _a(M,N)<=0 || _b(M,N)<=0) mooseError("CardiacNash2000Material: invalid material parameters.");
}

void
CardiacNash2000Material::computeQpStressProperties(const SymmTensor &C, const SymmTensor &E)
{
  // 2nd Piola-Kirchhoff stress tensor: T(MN) = 1/2[dW/dE(MN) + dW/dE(NM)] + [- p + Ta delta(M1) delta(N1) ] C^-1(MN)
  // We make use of dW/dE(MN) == dW/dE(NM) and will add active tension and pressure terms outside (see #CardiacMechanicsMaterial)
  SymmTensor T(0);
  // Derivative of T: D(MNPQ) = dT(MN)/dE(PQ)
  SymmGenericElasticityTensor D(0);
  // We will sum up the elastic energy contributions inside the loop
  Real W(0);

  for (unsigned int M=0;M<3;M++)
    for (unsigned int N=M;N<3;N++)
    {
      const Real k(_k(M,N));
      const Real e(std::abs(E(M,N)));
      const Real a(_a(M,N));
      const Real b(_b(M,N));
      if (e>a) mooseError("CardiacNash2000Material: E_{MN} >= a_{MN} - the strain is too large for this model");
      const Real d(std::abs(a - e));
      const Real f( b*e/d );
      const Real g( k*pow(d,-b) );

      const Real fact(M==N ? 1.0 : 0.5);

      T(M,N) = fact * g * e * ( 2+f );

      // insane loop to cover all elements of the symmetric 4th order tensor exactly once. The if-construct avoids douple-counting for major-symmetry pairs of MN<->PQ
      for (unsigned int P=M;P<3;P++)
        for (unsigned int Q=P;Q<3;Q++)
          if (P != M || Q >= N)
            D(M,N,P,Q) = fact * g * ( 2 + (4+e/d+f)*f );

      W += fact * g * e*e;
    }

  _stress[_qp] = STtoRTV(T);
  _stress_derivative[_qp] = D;
  _W[_qp] = W;
}

