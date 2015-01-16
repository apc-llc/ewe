#include "CardiacHolzapfel2009Material.h"
#include "TensorHelpers.h"

using namespace TensorHelpers;

template<>
InputParameters validParams<CardiacHolzapfel2009Material>()
{
  InputParameters params = validParams<CardiacMechanicsMaterial>();
  params.addRequiredParam<std::vector<Real> >("material_parameters", "Material parameters as given in Table 1 of [Holzapfel, 2009] in following order: a, b, a_f, b_f, a_s, b_s, a_fs, b_fs. Any a-parameter has to be given in kPa, any b is unitless.");
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
CardiacHolzapfel2009Material::computeQpStressProperties(const SymmTensor &C, const SymmTensor & /*E*/)
{
  const SymmTensor CC(square(C));
  // invariants (We will not need all of them. However, defining them avoids to forget any.)
  const Real I1(C.trace());
  /*const Real I2(0.5*(I1*I1-CC.trace()));*/
  /*const Real I3(_J[_qp]);               */
  const Real I4f(_Ef[_qp]*(C*_Ef[_qp]));
  const Real I4s(_Es[_qp]*(C*_Es[_qp]));
  /*const Real I4n(_En[_qp]*(C*_En[_qp])); */
  /*const Real I5f(_Ef[_qp]*(CC*_Ef[_qp]));*/
  /*const Real I5s(_Es[_qp]*(CC*_Es[_qp]));*/
  /*const Real I5n(_En[_qp]*(CC*_En[_qp]));*/
  const Real I8fs(_Ef[_qp]*(C*_Es[_qp]));
  /*const Real I8fn(_Ef[_qp]*(C*_En[_qp]));*/
  /*const Real I8sn(_Es[_qp]*(C*_En[_qp]));*/

  // the following will be needed in the stress as well as in the energy and stress_derivative
  const Real  i_term(   _p[A1 ]*std::exp(_p[B1 ]*(I1 -3)        ) );
  const Real  f_term( 2*_p[Af ]*std::exp(_p[Bf ]*(I4f-1)*(I4f-1)) );
  const Real  s_term( 2*_p[As ]*std::exp(_p[Bs ]*(I4s-1)*(I4s-1)) );
  const Real fs_term(   _p[Afs]*std::exp(_p[Bfs]* I8fs  * I8fs  ) );

  // elastic energy contribution
  _W[_qp] =  i_term             /(2*_p[B1 ])
         + ( f_term - 2*_p[Af ])/(2*_p[Bf ])
         + ( s_term - 2*_p[As ])/(2*_p[Bs ])
         + (fs_term - 2*_p[Afs])/(2*_p[Bfs]);

  // tensors for constructing stress and stress_derivative
  const SymmTensor EfEf(kron(_Ef[_qp]));
  const SymmTensor EsEs(kron(_Es[_qp]));
  const SymmTensor EfEs(kronSym(_Ef[_qp],_Es[_qp]));

  _stress[_qp] = scaledID(i_term) + EfEf*(I4f-1)*f_term + EsEs*(I4s-1)*s_term + EfEs*I8fs*fs_term;

  for (int M=0;M<3;M++)
    for (int N=M;N<3;N++)
      for (int P=0;P<3;P++)
        for (int Q=P;Q<3;Q++)
          _stress_derivative[_qp](M,N,P,Q) =                         _p[B1 ]  *  i_term *  _id(M,N)  * _id(P,Q)
                                            + (1 + (I4f-1)*(I4f-1)*2*_p[Bf ]) *  f_term * EfEf(M,N) * EfEf(P,Q)
                                            + (1 + (I4f-1)*(I4f-1)*2*_p[Bf ]) *  s_term * EsEs(M,N) * EsEs(P,Q)
                                            + (1 +  I8fs          *2*_p[Bfs]) * fs_term * EfEs(M,N) * EfEs(P,Q);
}

