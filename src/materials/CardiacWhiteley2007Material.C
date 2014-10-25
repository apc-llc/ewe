/****************************************************************/
/*                                                              */
/*  Implementation of Cardiac Tissue elasticity as given in     */
/*  [Whiteley2007]                                              */
/*                                                              */
/****************************************************************/

#include "CardiacWhiteley2007Material.h"
#include "ColumnMajorMatrix.h"
#include "CardiacSolidMechanicsMaterial.h"
#include "SymmOrthotropicElasticityTensor.h"
#include "VolumetricModel.h"

template<>
InputParameters validParams<CardiacWhiteley2007Material>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredCoupledVar("disp_x", "The x displacement");
  params.addRequiredCoupledVar("disp_y", "The y displacement");
  params.addRequiredCoupledVar("disp_z", "The z displacement");

  params.addRequiredParam<std::vector<Real> >("k_MN", "Material parameters k_MN in following order: k_11, k_22, k_33, k_12, k_23, k_13");
  params.addRequiredParam<std::vector<Real> >("a_MN", "Material parameters a_MN in following order: a_11, a_22, a_33, a_12, a_23, a_13");
  params.addRequiredParam<std::vector<Real> >("b_MN", "Material parameters b_MN in following order: b_11, b_22, b_33, b_12, b_23, b_13");

  params.addCoupledVar("Ta", 0, "The (position dependent) active tension in fibre direction that will finally drive contraction, see (8) in [Whiteley 2007]. Default is Ta=0, i.e. no active tension anywhere.");
  params.addCoupledVar("p", 0, "Hydrostatic pressure that acts as a Lagrange multiplier to ensure incompressibility. Works best with PressureLagrangeMultiplier kernel.");
  return params;
}

CardiacWhiteley2007Material::CardiacWhiteley2007Material(const std::string  & name,
                                                 InputParameters parameters)
  :Material(name, parameters),
   _grad_disp_x(coupledGradient("disp_x")),
   _grad_disp_y(coupledGradient("disp_y")),
   _grad_disp_z(coupledGradient("disp_z")),
   _k(SymmTensor(getParam<std::vector<Real> >("k_MN"))),
   _a(SymmTensor(getParam<std::vector<Real> >("a_MN"))),
   _b(SymmTensor(getParam<std::vector<Real> >("b_MN"))),
   _stress(declareProperty<RealTensorValue>("stress")),
   _stress_derivative(declareProperty<SymmGenericElasticityTensor>("stress_derivative")),
   _J(declareProperty<Real>("det_displacement_gradient")),
   _Rf(getMaterialProperty<RealTensorValue>("R_fibre")),
   _has_Ta(isCoupled("Ta")),
   _Ta(_has_Ta ? coupledValue("Ta") : _zero),
   _has_p(isCoupled("p")),
   _p(_has_p ? coupledScalarValue("p") : _zero),
   _id(1, 1, 1, 0, 0, 0)
{}

const RealTensorValue CardiacWhiteley2007Material::STtoRTV(const SymmTensor & A) const
{
  RealTensorValue B;
  B(0,0) = A(0,0);
  B(1,1) = A(1,1);
  B(2,2) = A(2,2);
  B(0,1) = B(1,0) = A(0,1);
  B(0,2) = B(2,0) = A(0,2);
  B(1,2) = B(2,1) = A(1,2);
  return B;
}

const SymmGenericElasticityTensor CardiacWhiteley2007Material::STtoSGET(const SymmTensor & A) const
{
  SymmGenericElasticityTensor B;
  B(0,0,0,0) = A(0,0);
  B(1,1,1,1) = A(1,1);
  B(2,2,2,2) = A(3,3);
  B(2,3,2,3) = A(1,2);
  B(1,3,1,3) = A(0,2);
  B(1,2,1,2) = A(0,1);
  return B;
}

/*
 * computes outer.transpose() * inner * outer
 * TODO: this should be possible in a more efficient way as the resulting matrix is symmetric
 */
const SymmTensor CardiacWhiteley2007Material::symmProd(const RealTensorValue & outer, const SymmTensor & inner) const
{
  RealTensorValue r(outer.transpose() * STtoRTV(inner) * outer);
  return SymmTensor(r(0,0), r(1,1), r(2,2), r(0,1), r(1,2), r(0,2) );
}

/*
 * computes outer.transpose() * outer
 * TODO: this should be possible in a more efficient way as the resulting matrix is symmetric
 */
const SymmTensor CardiacWhiteley2007Material::symmProd(const RealTensorValue & outer) const
{
  RealTensorValue r(outer.transpose() * outer);
  return SymmTensor(r(0,0), r(1,1), r(2,2), r(0,1), r(1,2), r(0,2) );
}

/*
 * computes C^-1 using the already known det(C)
 */
const SymmTensor CardiacWhiteley2007Material::symmInv(const SymmTensor & C, const Real det) const
{
  SymmTensor Cinv(/* 00 */ C(0,0)*C(1,1)-C(1,2)*C(1,2),
                  /* 11 */ C(0,0)*C(0,0)-C(0,2)*C(0,2),
                  /* 22 */ C(0,0)*C(1,1)-C(0,1)*C(0,1),
                  /* 01 */ C(0,2)*C(1,2)-C(0,0)*C(0,1),
                  /* 12 */ C(0,1)*C(0,2)-C(0,0)*C(1,2),
                  /* 02 */ C(0,1)*C(1,2)-C(0,2)*C(1,1));
  return Cinv * (1./det);
}

void
CardiacWhiteley2007Material::computeQpProperties()
{
  // TODO: verify that all rotations are done in the correct direction, i.e. where do you have to use _Rf or _Rf.transpose() ?
  const RealTensorValue R(_Rf[_qp]);

  // local deformation gradient tensor: F(ij) = dx(i)/dX(j)
  // Attention: This is not the displacement gradient:
  //               du(i)/dX(j) = d[x(i)-X(i)]/dX(j) = F(ij) - delta(ij)
  // Thus, as we are working on displacements, we have to add unity to the diagonal
  // (every more elegant way I could think of was also less efficient than this one..)
  const RealTensorValue F(_grad_disp_x[_qp](0) + 1, _grad_disp_x[_qp](1),     _grad_disp_x[_qp](2),
                          _grad_disp_y[_qp](0),     _grad_disp_y[_qp](1) + 1, _grad_disp_y[_qp](2),
                          _grad_disp_z[_qp](0),     _grad_disp_z[_qp](1),     _grad_disp_z[_qp](2) + 1);
  // ...its determinant is a measure for local volume changes (is needed in kernel that ensures incompressibility via hydrostatic pressure/Lagrange multiplier p)
  _J[_qp] = F.det();
  // From here on, we go over to fibre coordinates, i.e. for C, Cinv, E, T
  // Cauchy-Green deformation tensor in fibre coordinates: C = R^T F^T F R
  const SymmTensor C(symmProd(R, symmProd(F)));
  // .. and its inverse (note that det(C_fibre) = det(C) = det(F^T)*det(F) = det(F)^2, since det(R)==1
  const SymmTensor Cinv(symmInv(C, _J[_qp]*_J[_qp]));
  // Lagrange-Green strain tensor
  const SymmTensor E( (C - _id) * 0.5 );
  
  // 2nd Piola-Kirchhoff stress tensor: T(MN) = 1/2[dW/dE(MN) + dW/dE(NM)] + [- p + Ta delta(M1) delta(N1) ] Cinv(MN)
  // We make use of dW/dE(MN) == dW/dE(NM) and will add active tension and pressure terms later
  SymmTensor T;
  // Derivative of T: D(MNPQ) = dT(MN)/dE(PQ)
  // We make use of the symmetry of T and the fact that for our W, dT(MN)/dE(PQ) \propto delta(MP)delta(PQ) if p and Ta are omitted.
  // Thus, a symmetric second-order tensor is sufficient here. Ta and p will be added later, again.
  SymmTensor D;

  for (int M=0;M<3;M++)
    for (int N=M;N<3;N++)
      if (E(M,N) > 0) {
        const Real a(_a(M,N));
        const Real b(_b(M,N));
        const Real k(_k(M,N));
        const Real e(E(M,N));
        const Real d( a - e );
        if (d <= 0) mooseError("CardiacWhiteley2007Material: E_{MN} >= a_{MN} - the strain is too large for this model");
        const Real f( b*e/d );
        const Real g( k/pow(d,b) );

        T(M,N) = g * e * ( 2+f );
        D(M,N) = g * ( 2 + (4+e/d+f)*f );
      } else {
        T(M,N) = 0.;
        D(M,N) = 0.;
      }

  // The following renders D asymmetric wrt. (MN)<->(PQ), i.e. we need the full fourth order tensor here.
  _stress_derivative[_qp] = STtoSGET(D);
  // Add hydrostatic pressure as Lagrange multiplier to ensure incompressibility
  if (_has_p) {
    T -= Cinv*_p[0]; // TODO: is [0] correct for scalar variables?
    // for the derivative of T, things do become slightly complicated as we have to do
    // TODO: _stress_derivative[_qp](MNPQ) += 2 * _p[0] * invC(M,P) * invC(Q,N);
    // the pressure term is symmetric in Q<->N and in M<->P, i.e.   C(MNPQ)=C(MQPN) and C(MNPQ)=C(PNMQ) due to symmetry of invC
  }

  // Te following renders T asymmetric.
  _stress[_qp] = STtoRTV(T);
  // Add active tension in fibre direction
  if (_has_Ta) {
    _stress[_qp] += RealTensorValue(_Ta[_qp]*Cinv(0,0), _Ta[_qp]*Cinv(0,1), _Ta[_qp]*Cinv(0,2), 0, 0, 0, 0, 0, 0);
    // TODO: _stress_derivative[_qp](MNPQ) -= 2 * _Ta[_qp] delta(M1) delta(N1) * invC(M,P) * invC(Q,N);
  }

  // rotate everything back into outer coordinate system
  _stress[_qp] = R * _stress[_qp] * R.transpose();
  // TODO: dito for _stress_derivative[_qp](MNPQ) = R(Mm)R(Nn)R(Pp)R(Qq)_stress_derivative[_qp](mnpq)

}

