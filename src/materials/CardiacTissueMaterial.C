/****************************************************************/
/*                                                              */
/*  Implementation of Cardiac Tissue elasticity as given in     */
/*  [Whiteley2007]                                              */
/*                                                              */
/****************************************************************/

#include "CardiacTissueMaterial.h"

template<>
InputParameters validParams<CardiacTissueMaterial>()
{
  InputParameters params = validParams<Material>();

  // Vectors for Linear Interpolation
  params.addRequiredParam<std::vector<Real> >("k_MN", "Material parameters k_MN in following order: k_11, k_22, k_33, k_12, k_23, k_13");
  params.addRequiredParam<std::vector<Real> >("a_MN", "Material parameters a_MN in following order: a_11, a_22, a_33, a_12, a_23, a_13");
  params.addRequiredParam<std::vector<Real> >("b_MN", "Material parameters b_MN in following order: b_11, b_22, b_33, b_12, b_23, b_13");

  params.addCoupledVar("Ta", 0, "The (position dependent) active strain that will finally drive contraction. Default is Ta=0, i.e. no active strain anywhere.");
  params.addCoupledVar("x",  "Deformed body coordinate x = f(X)");
  params.addCoupledVar("y",  "Deformed body coordinate y = f(Y)");
  params.addCoupledVar("z",  "Deformed body coordinate z = f(Z)");
  params.addCoupledVar("p",  "Pressure (Lagrange multiplier)");

  return params;
}

CardiacTissueMaterial::CardiacTissueMaterial(const std::string & name,
                                 InputParameters parameters) :
    Material(name, parameters)
    ,
    // Declare that this material is going to provide a RealTensorValue
    // valued property named "FibreOrientation" that Kernels can use.
    _P(declareProperty<RealTensorValue>("FibreOrientation"))          // TODO: these have to be set in the constructor appropriately
    ,
    // Declare that this material is going to provide a RealTensorValue
    // valued property named "deformation_gradient" that kernels can use.
    _F(declareProperty<RealTensorValue>("deformation_gradient"))
    ,
    // Declare that this material is going to provide a RealTensorValue
    // valued property named "PiolaKirchoff2nd" that Kernels can use.
    _T(declareProperty<RealTensorValue>("PiolaKirchoff2nd"))
    ,
    _k(SymmTensor(getParam<std::vector<Real> >("k_MN")))
    ,
    _a(SymmTensor(getParam<std::vector<Real> >("a_MN")))
    ,
    _b(SymmTensor(getParam<std::vector<Real> >("b_MN")))
{}

void
CardiacTissueMaterial::computeQpProperties()
{
  // TODO: for now we set fibre orientation identical to original coordinates
  _P[_qp] = RealTensorValue(1., 0., 0., 0., 1., 0., 0., 0., 1.);

  // deformation gradient is straightforward
  _F [_qp] = RealTensorValue(coupledGradient("x")[_qp],
                             coupledGradient("y")[_qp],
                             coupledGradient("z")[_qp]);
                             
  // Cauchy Green Deformation tensor in fibre-oriented coordinates C* = P^t F^tF P
  RealTensorValue Cstar = _P[_qp].transpose() * _F[_qp].transpose() * _F[_qp] * _P[_qp]; // TODO: efficiency: we know that F^tF is symmetric but compute all 9 elements instead of the 6 unique ones

  // ...and its inverse...
  RealTensorValue Cstar_inv = Cstar.inverse(); // TODO: ditto
  
  // Lagrange-Green strain tensor
  RealTensorValue Estar = (Cstar - RealTensorValue(1., 0., 0., 0., 1., 0., 0., 0., 1.)) * 0.5;  //TODO: also symmetric
  
  // derivative of elastic energy
  RealTensorValue dWdE;
  
  for (int M=0;M<3;M++)
    for (int N=0;N<3;N++) {
      if (Estar(M,N) >= 0) {
        dWdE(M,N) = _k(M, N) * Estar(M,N) / pow(abs(_a(M,N) - Estar(M,N)), _b(M,N));
      } else {
        dWdE(M,N) = 0.;
      }
    }
  
  // 2nd Piola Kirchoff tensor
  // elastic forces
  RealTensorValue Tstar = (dWdE + dWdE.transpose())*0.5 - Cstar_inv*coupledValue("p")[_qp];  // TODO: also symmetric
  // active strain
  Tstar(0, 0) += coupledValue("Ta")[_qp] * Cstar_inv(0,0);
  // rotate back into outer coordinate system
  _T[_qp] = _P[_qp] * Tstar * _P[_qp].transpose(); // TODO: also symmetric

}
