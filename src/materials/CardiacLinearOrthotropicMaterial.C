/*************************************
 * Template for this file came from moose/modules/solid_mechanics/src/materials/total_strain/LinearIsotropicMaterial.C
 *************************************/

#include "CardiacLinearOrthotropicMaterial.h"
#include "ColumnMajorMatrix.h"
#include "CardiacSolidMechanicsMaterial.h"
#include "SymmOrthotropicElasticityTensor.h"
#include "VolumetricModel.h"

template<>
InputParameters validParams<CardiacLinearOrthotropicMaterial>()
{
  InputParameters params = validParams<CardiacSolidMechanicsMaterial>();
  params.addRequiredParam<Real>("youngs_modulus", "Young's Modulus");
  params.addRequiredParam<Real>("poissons_ratio", "Poisson's Ratio");
  return params;
}

CardiacLinearOrthotropicMaterial::CardiacLinearOrthotropicMaterial(const std::string  & name,
                                                 InputParameters parameters)
  :CardiacSolidMechanicsMaterial(name, parameters),
   _youngs_modulus(getParam<Real>("youngs_modulus")),
   _poissons_ratio(getParam<Real>("poissons_ratio")),
   _local_elasticity_tensor(NULL)
{
  SymmOrthotropicElasticityTensor * iso_elasticity_tensor = new SymmOrthotropicElasticityTensor;
  iso_elasticity_tensor->setYoungsModulus(_youngs_modulus);
  iso_elasticity_tensor->setPoissonsRatio(_poissons_ratio);

  _local_elasticity_tensor = iso_elasticity_tensor;

  _pi = 3.14159;
  _tol = 1.0e-5;
}

CardiacLinearOrthotropicMaterial::~CardiacLinearOrthotropicMaterial()
{
  delete _local_elasticity_tensor;
}

void
CardiacLinearOrthotropicMaterial::computeProperties()
{
  for (_qp=0; _qp < _qrule->n_points(); ++_qp)
  {
    _local_elasticity_tensor->calculate(_qp);

    _elasticity_tensor[_qp] = *_local_elasticity_tensor;

    SymmTensor strn( _grad_disp_x[_qp](0),
                     _grad_disp_y[_qp](1),
                     _grad_disp_z[_qp](2),
                     0.5*(_grad_disp_x[_qp](1)+_grad_disp_y[_qp](0)),
                     0.5*(_grad_disp_y[_qp](2)+_grad_disp_z[_qp](1)),
                     0.5*(_grad_disp_z[_qp](0)+_grad_disp_x[_qp](2)) );

    computeStress(strn, _stress[_qp]);
  }
}

void
CardiacLinearOrthotropicMaterial::computeStress(const SymmTensor & strain,
                                       SymmTensor & stress)
{
  // Multiplier that zeros out stiffness
  Real h = (1.0 + std::cos(_pi*_c[_qp]))/2.0;
  if (h < _tol)
    h = _tol;

  //Jacobian multiplier of the stress
  _Jacobian_mult[_qp] = *_local_elasticity_tensor*h;

  // Save that off as the elastic strain
  _elastic_strain[_qp] = strain;

  // Create column vector
  // C * e
  stress = (*_local_elasticity_tensor)*h*strain;
}

