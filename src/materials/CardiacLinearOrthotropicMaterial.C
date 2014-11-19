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
   _Rf(getMaterialProperty<RealTensorValue>("R_fibre")),
   _local_elasticity_tensor(NULL)
{
  if (_youngs_moduli.size()   != 3) mooseError("CardiacLinearOrthotropicMaterial: youngs_moduli must contain exactly 3 entries.");
  if (_poissons_ratios.size() != 3) mooseError("CardiacLinearOrthotropicMaterial: poissons_ratios must contain exactly 3 entries.");
  if (_shear_moduli.size()    != 3) mooseError("CardiacLinearOrthotropicMaterial: shear_moduli must contain exactly 3 entries.");

  SymmOrthotropicElasticityTensor * ortho_elasticity_tensor = new SymmOrthotropicElasticityTensor;
  ortho_elasticity_tensor->setYoungsModuli(_youngs_moduli[0], _youngs_moduli[1], _youngs_moduli[2]);
  ortho_elasticity_tensor->setPoissonsRatios(_poissons_ratios[0], _poissons_ratios[1], _poissons_ratios[2]);
  ortho_elasticity_tensor->setShearModuli(_shear_moduli[0], _shear_moduli[1], _shear_moduli[2]);

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
  for (_qp=0; _qp < _qrule->n_points(); ++_qp)
  {
    // since for the elasticity tensor _constant=true (default) and is independent of _qp,
    // it is only recomputed once, any subsequent calls reduce to a no-operation
    _local_elasticity_tensor->calculate(_qp);
  
    // store elasticity tensor as material property...
    _elasticity_tensor[_qp] = *_local_elasticity_tensor;

    // ...and rotate it to the outer coordinate system
    ColumnMajorMatrix R_f_9x9(9,9);
    _elasticity_tensor[_qp].form9x9Rotation( ColumnMajorMatrix(_Rf[_qp]), R_f_9x9 );
    _elasticity_tensor[_qp].rotateFromLocalToGlobal( R_f_9x9);

    SymmTensor strain( _grad_disp_x[_qp](0),
                       _grad_disp_y[_qp](1),
                       _grad_disp_z[_qp](2),
                       0.5*(_grad_disp_x[_qp](1)+_grad_disp_y[_qp](0)),
                       0.5*(_grad_disp_y[_qp](2)+_grad_disp_z[_qp](1)),
                       0.5*(_grad_disp_z[_qp](0)+_grad_disp_x[_qp](2)) );

    /* compute the stress */
    // Multiplier that zeros out stiffness
    Real h = (1.0 + std::cos(_pi*_c[_qp]))/2.0;
    if (h < _tol)  h = _tol;
    //Jacobian multiplier of the stress
    _Jacobian_mult[_qp] = _elasticity_tensor[_qp]*h;
    // Save off the elastic strain
    _elastic_strain[_qp] = strain;
    // Create column vector C*e
    _stress[_qp] = (*_local_elasticity_tensor)*h*strain;
  }
}

