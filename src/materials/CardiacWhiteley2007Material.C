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
   _Rf(getMaterialProperty<RealTensorValue>("R_fibre")),
   _has_Ta(isCoupled("Ta")),
   _Ta(coupledValue("Ta")),
   _id(1, 1, 1, 0, 0, 0)
{}

const RealTensorValue CardiacWhiteley2007Material::fromSymm(const SymmTensor & A)
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

/*
 * computes outer.transpose() * inner * outer
 * TODO: this should be possible in a much more efficient way
 */
const SymmTensor CardiacWhiteley2007Material::symmProd(const RealTensorValue & outer, const SymmTensor & inner)
{
  RealTensorValue in(fromSymm(inner));
  RealTensorValue r(outer.transpose() * in * outer);
  return SymmTensor(r(0,0), r(1,1), r(2,2), r(0,1), r(1,2), r(0,2) );
}

void
CardiacWhiteley2007Material::computeQpProperties()
{
  // TODO: verify that all rotations are done in the correct direction, i.e. where do you have to use _Rf or _Rf.transpose() ?

  // local deformation gradient tensor: insert displacement gradient row-wise
  const RealTensorValue F(_grad_disp_x[_qp],
                          _grad_disp_y[_qp],
                          _grad_disp_z[_qp]);
  // Cauchy-Green deformation tensor
  const SymmTensor C(symmProd(F, _id));
  // Lagrange-Green strain tensor
  const SymmTensor E( (C - _id) * 0.5 );
  // Lagrange-Green strain tensor in fibre coordinates
  const SymmTensor Estar(symmProd(_Rf[_qp], E));
  
  // Derivative of strain energy density W wrt Estar
  SymmTensor dWdE;
  for (int i=0;i<3;i++)
    for (int j=i;j<3;j++)
      if (Estar(i,j) > 0) {
        const Real d(abs(_a(i,j) - Estar(i,j)));
        dWdE(i,j) = _k(i, j) * Estar(i,j) / pow(d, _b(i,j))  *  ( 2 + _b(i,j)*Estar(i,j)/d );
      } else
        dWdE(i,j) = 0.;
  // rotate back into outer coordinate system
  dWdE = symmProd(_Rf[_qp].transpose(), dWdE);

  const Real p = 0; // TODO: p is the hydrostatic pressure / Lagrange multiplier to guarantee incompressibility
  _stress[_qp] = _id*(-p) + symmProd(F.transpose(), dWdE);
  
  // compute active tension in fibre direction, rotate into outer coordinates and add to stress if necessary
  if (_has_Ta) {
    SymmTensor Ta( symmProd( _Rf[_qp], SymmTensor(_Ta[_qp], 0, 0, 0, 0, 0) ) );
    _stress[_qp] += Ta;
  }

  /* TODO: currently, though being possibly required (e.g. by the StressDivergence kernel), we do not set the following
     // store elasticity tensor as material property...
     _elasticity_tensor[_qp] =
     //Jacobian multiplier of the stress
     _Jacobian_mult[_qp] =
  // Save off the elastic strain
     _elastic_strain[_qp] =   SymmTensor( _grad_disp_x[_qp](0),
                                          _grad_disp_y[_qp](1),
                                          _grad_disp_z[_qp](2),
                                          0.5*(_grad_disp_x[_qp](1)+_grad_disp_y[_qp](0)),
                                          0.5*(_grad_disp_y[_qp](2)+_grad_disp_z[_qp](1)),
                                          0.5*(_grad_disp_z[_qp](0)+_grad_disp_x[_qp](2)) );;
  */
}

