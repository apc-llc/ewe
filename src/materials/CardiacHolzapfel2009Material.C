#include "CardiacHolzapfel2009Material.h"
#include "TensorHelpers.h"

using namespace TensorHelpers;

template<>
InputParameters validParams<CardiacHolzapfel2009Material>()
{
  InputParameters params = validParams<CardiacMechanicsMaterial>();
  params.addRequiredParam<std::vector<Real> >("material_parameters", "Material parameters as given in Table 1 of [Holzapfel 2009] in following order: a, b, a_f, b_f, a_s, b_s, a_fs, b_fs");
  return params;
}

CardiacHolzapfel2009Material::CardiacHolzapfel2009Material(const std::string  & name,
                                                 InputParameters parameters)
  :CardiacMechanicsMaterial(name, parameters),
   _p(getParam<std::vector<Real> >("material_parameters"))
{
  if (_p.size()!=8)
    mooseError("CardiacHolzapfel2009Material: invalid number of entries in material_parameters.");
}

void
CardiacHolzapfel2009Material::computeQpStressProperties(const SymmTensor &C, const SymmTensor &E)
{
  const SymmTensor CC(square(C));
  // invariants (We will not need all of them. However, defining them avoids to forget any. The compiler will optimize out the unused.)
  const Real I1(C.trace());
  const Real I2(0.5*(I1*I1-CC.trace()));
  const Real I3(_J[_qp]);
  const Real I4f(_Ef[_qp]*(C*_Ef[_qp]));
  const Real I4s(_Es[_qp]*(C*_Es[_qp]));
  const Real I4n(_En[_qp]*(C*_En[_qp]));
  const Real I5f(_Ef[_qp]*(CC*_Ef[_qp]));
  const Real I5s(_Es[_qp]*(CC*_Es[_qp]));
  const Real I5n(_En[_qp]*(CC*_En[_qp]));
  const Real I8fs(_Ef[_qp]*(C*_Es[_qp]));
  const Real I8fn(_Ef[_qp]*(C*_En[_qp]));
  const Real I8sn(_Es[_qp]*(C*_En[_qp]));

  // elastic energy contribution
  Real W(  _p[A1 ]/(2*_p[B1 ]) *  std::exp(_p[B1 ]*(I1-3)         )
         + _p[Af ]/(2*_p[Bf ]) * (std::exp(_p[Bf ]*(I4f-1)*(I4f-1)) - 1)
         + _p[As ]/(2*_p[Bs ]) * (std::exp(_p[Bs ]*(I4s-1)*(I4s-1)) - 1)
         + _p[Afs]/(2*_p[Bfs]) * (std::exp(_p[Bfs]* I8fs  * I8fs   ) - 1)
        );

  const SymmTensor EfEf(kron(_Ef[_qp]));
  const SymmTensor EsEs(kron(_Es[_qp]));
  const SymmTensor EfEs(kronSym(_Ef[_qp],_Es[_qp]));

  SymmTensor T( scaledID( _p[A1 ]         * std::exp(_p[B1 ]*(I1-3))        )
               + EfEf * 2*_p[Af ]*(I4f-1) * std::exp(_p[Bf ]*(I4f-1)*(I4f-1))
               + EsEs * 2*_p[As ]*(I4s-1) * std::exp(_p[Bs ]*(I4s-1)*(I4s-1))
               + EfEs *   _p[Afs]* I8fs   * std::exp(_p[Bfs]* I8fs  * I8fs  )
              );

  _stress[_qp] = STtoRTV(T);
// TODO  _stress_derivative[_qp] = STtoSGET(D);
  _W[_qp] = W;

}

