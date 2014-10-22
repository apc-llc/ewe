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
  params.addRequiredParam<std::vector<Real> >("youngs_moduli", "Young's Moduli: E_1, E_2, E_3");
  params.addRequiredParam<std::vector<Real> >("poissons_ratios", "Poisson's Ratios: nu_12, nu_13, nu_23");
  params.addRequiredParam<std::vector<Real> >("shear_moduli", "Shear Moduli: G_12, G_13, G_23");
  return params;
}

CardiacLinearOrthotropicMaterial::CardiacLinearOrthotropicMaterial(const std::string  & name,
                                                 InputParameters parameters)
  :CardiacSolidMechanicsMaterial(name, parameters),
   _youngs_moduli(getParam<std::vector<Real> >("youngs_moduli")),
   _poissons_ratios(getParam<std::vector<Real> >("poissons_ratios")),
   _shear_moduli(getParam<std::vector<Real> >("shear_moduli")),
   _local_elasticity_tensor(NULL)
{
  if (_youngs_moduli.size()   != 3) mooseError("CardiacLinearOrthotropicMaterial: youngs_moduli must contain exactly 3 entries.");
  if (_poissons_ratios.size() != 3) mooseError("CardiacLinearOrthotropicMaterial: poissons_ratios must contain exactly 3 entries.");
  if (_shear_moduli.size()    != 3) mooseError("CardiacLinearOrthotropicMaterial: shear_moduli must contain exactly 3 entries.");

  SymmOrthotropicElasticityTensor * ortho_elasticity_tensor = new SymmOrthotropicElasticityTensor;
  ortho_elasticity_tensor->setYoungsModuli(_youngs_moduli[0], _youngs_moduli[1], _youngs_moduli[2]);
  ortho_elasticity_tensor->setPoissonsRatios(_poissons_ratios[0], _poissons_ratios[1], _poissons_ratios[2]);

  // This is the elasticity tensor expressed in the fibre coordinate system.
  // Thus, it is spatially constant and will have to be rotated appropriately to yield the correct relation between local stresses and strains.
  _local_elasticity_tensor = ortho_elasticity_tensor;

  _pi  = 3.14159;
  _tol = 1.0e-5;
}

CardiacLinearOrthotropicMaterial::~CardiacLinearOrthotropicMaterial()
{
  delete _local_elasticity_tensor;
}

void
CardiacLinearOrthotropicMaterial::computeProperties()
{
  // Multiplier that zeros out stiffness
  Real h = (1.0 + std::cos(_pi*_c[_qp]))/2.0;
  if (h < _tol)  h = _tol;
  
  for (_qp=0; _qp < _qrule->n_points(); ++_qp)
  {
    // since for the elasticity tensor _constant=true (default) and is independent of _qp,
    // it is only recomputed once, any subsequent calls reduce to a no-operation
    _local_elasticity_tensor->calculate(_qp);
  
    // TODO: have to rotate this tensor appropriately somewhere here...
    // two options:
    // 1. _elasticity_tensor[_qp] = R* (*_local_elasticity_tensor); rotate back upon stress computation
    // 2. rotate explicitly upon every use of elasticity_tensor[_qp]
    //
    // To decide, we have to know:
    // A. where is _elasticity_tensor[_qp] used
    // B. where is _Jacobian_mult[_qp] used
    //
    // NO: we have to rotate stress and strain explicitly, since these are the 3x3-matrices while the elasticity tensor is 6x6

    // store elasticity tensor as material property
    _elasticity_tensor[_qp] = *_local_elasticity_tensor;

    SymmTensor strain( _grad_disp_x[_qp](0),
                       _grad_disp_y[_qp](1),
                       _grad_disp_z[_qp](2),
                       0.5*(_grad_disp_x[_qp](1)+_grad_disp_y[_qp](0)),
                       0.5*(_grad_disp_y[_qp](2)+_grad_disp_z[_qp](1)),
                       0.5*(_grad_disp_z[_qp](0)+_grad_disp_x[_qp](2)) );

    /* compute the stress */
    //Jacobian multiplier of the stress
    _Jacobian_mult[_qp] = *_local_elasticity_tensor*h;
    // Save that off as the elastic strain
    _elastic_strain[_qp] = strain;
    // Create column vector C*e
    _stress[_qp] = (*_local_elasticity_tensor)*h*strain;
  }
}

