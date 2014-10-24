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
  InputParameters params = validParams<CardiacSolidMechanicsMaterial>();
  params.addRequiredParam<std::vector<Real> >("k_MN", "Material parameters k_MN in following order: k_11, k_22, k_33, k_12, k_23, k_13");
  params.addRequiredParam<std::vector<Real> >("a_MN", "Material parameters a_MN in following order: a_11, a_22, a_33, a_12, a_23, a_13");
  params.addRequiredParam<std::vector<Real> >("b_MN", "Material parameters b_MN in following order: b_11, b_22, b_33, b_12, b_23, b_13");

  params.addCoupledVar("Ta", 0, "The (position dependent) active tension in fibre direction that will finally drive contraction, see (8) in [Whiteley 2007]. Default is Ta=0, i.e. no active tension anywhere.");
  return params;
}

CardiacWhiteley2007Material::CardiacWhiteley2007Material(const std::string  & name,
                                                 InputParameters parameters)
  :CardiacSolidMechanicsMaterial(name, parameters),
    _k(SymmTensor(getParam<std::vector<Real> >("k_MN"))),
    _a(SymmTensor(getParam<std::vector<Real> >("a_MN"))),
    _b(SymmTensor(getParam<std::vector<Real> >("b_MN"))),
   _J(declareProperty<Real>("det_displacement_gradient")),
   _Rf(getMaterialProperty<RealTensorValue>("R_fibre")),
   _has_Ta(isCoupled("Ta")),
   _Ta(coupledValue("Ta")),
   _has_p(isCoupled("p")),
   _p(coupledScalarValue("p")),
   _id(1, 1, 1, 0, 0, 0)
{}

const RealTensorValue CardiacWhiteley2007Material::STtoRTV(const SymmTensor & A)
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

const SymmElasticityTensor CardiacWhiteley2007Material::STtoSET(const SymmTensor & A)
{
  SymmElasticityTensor B;
  // the function expects  C1111,  C1122, C1133, C2222,  C2233, C3333,  C2323,      C1313,      C1212
  const Real elements[] = {A(0,0), 0.,    0.,    A(1,1), 0.,    A(2,2), 0.5*A(1,2), 0.5*A(0,2), 0.5*A(0,1)};
  B.fillFromInputVector(std::vector<Real>(elements, elements + sizeof elements / sizeof elements[0]), false);
  return B;
}

/*
 * computes outer.transpose() * inner * outer
 * TODO: this should be possible in a more efficient way as the resulting matrix is symmetric
 */
const SymmTensor CardiacWhiteley2007Material::symmProd(const RealTensorValue & outer, const SymmTensor & inner)
{
  RealTensorValue r(outer.transpose() * STtoRTV(inner) * outer);
  return SymmTensor(r(0,0), r(1,1), r(2,2), r(0,1), r(1,2), r(0,2) );
}

/*
 * computes outer.transpose() * outer
 * TODO: this should be possible in a more efficient way as the resulting matrix is symmetric
 */
const SymmTensor CardiacWhiteley2007Material::symmProd(const RealTensorValue & outer)
{
  RealTensorValue r(outer.transpose() * outer);
  return SymmTensor(r(0,0), r(1,1), r(2,2), r(0,1), r(1,2), r(0,2) );
}

void
CardiacWhiteley2007Material::computeQpProperties()
{
  // TODO: verify that all rotations are done in the correct direction, i.e. where do you have to use _Rf or _Rf.transpose() ?
  const RealTensorValue R(_Rf[_qp]);

  // local deformation gradient tensor: insert displacement gradient row-wise...
  const RealTensorValue F(_grad_disp_x[_qp],
                          _grad_disp_y[_qp],
                          _grad_disp_z[_qp]);
  // ...its determinant is a measure for local volume changes (is needed in kernel that ensures incompressibility via hydrostatic pressure/Lagrange multiplier p)
  _J[_qp] = F.det();
  // Cauchy-Green deformation tensor C = F^T F
  const SymmTensor C(symmProd(F));
  // Lagrange-Green strain tensor in fibre coordinates
  const SymmTensor E(symmProd(R, (C - _id) * 0.5 ));
  
  // Derivative of strain energy density W wrt E
  SymmTensor dWdE(0);
  SymmTensor ddWdEdE(0);

  for (int M=0;M<3;M++)
    for (int N=M;N<3;N++)
      if (E(M,N) > 0)
      {
        const Real a(_a(M,N));
        const Real b(_b(M,N));
        const Real k(_k(M,N));
        const Real e(E(M,N));
        const Real d( a - e );
        if (d <= 0) mooseError("CardiacWhiteley2007Material: E_{MN} >= a_{MN} - the strain is too large for this model");
        const Real f( b*e/d );
        const Real g( k/pow(d,b) );

        dWdE(M,N)    = g * e * ( 2+f );
        ddWdEdE(M,N) = g * ( 2 + (4+e/d+f)*f );

      } else {
        dWdE(M,N) = 0.;
        ddWdEdE(M,N) = 0.;
      }

  // rotate back into outer coordinate system and convert from 2nd Piola-Kirchhoff stress to Cauchy stress: sigma = (F R) T (F R)^T
  _stress[_qp] = symmProd( (F*R).transpose(), dWdE);
  //Jacobian multiplier of the stress
  _Jacobian_mult[_qp] = STtoSET( symmProd( (F*R).transpose(), ddWdEdE) );

  // add hydrostatic pressure as Lagrange multiplier to ensure incompressibility
  if (_has_p) {
    _stress[_qp] -= _id*_p[0];
    // TODO: pressure component is missing in ddWdEdE
    // rDTdE(M,N,P,Q) += 2 * _p[0] * invC_transformed(M,P) * invC_transformed(Q,N);
  }

  // add active tension in fibre direction, rotate into outer coordinates and add to stress if necessary
  if (_has_Ta) {
    SymmTensor Ta( symmProd( R, SymmTensor(_Ta[_qp], 0, 0, 0, 0, 0) ) );
    _stress[_qp] += Ta;
    // TODO: how does this go into the Jacobian or ddWdEdE?
    // _Jacobian_mult[_qp] += ...
  }

  // To the best of my knowledge, the following are currently only needed for output purposes
  // TODO: store elasticity tensor as material property...
  // _elasticity_tensor[_qp] =
  // Save off the elastic strain
  _elastic_strain[_qp] =   SymmTensor( _grad_disp_x[_qp](0),
                                       _grad_disp_y[_qp](1),
                                       _grad_disp_z[_qp](2),
                                       0.5*(_grad_disp_x[_qp](1)+_grad_disp_y[_qp](0)),
                                       0.5*(_grad_disp_y[_qp](2)+_grad_disp_z[_qp](1)),
                                       0.5*(_grad_disp_z[_qp](0)+_grad_disp_x[_qp](2)) );;
}

