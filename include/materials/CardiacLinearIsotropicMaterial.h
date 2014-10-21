/*************************************
 * Template for this file came from moose/modules/solid_mechanics/include/materials/total_strain/LinearIsotropicMaterial.h
 *************************************/

#ifndef CardiacLinearIsotropicMaterial_H
#define CardiacLinearIsotropicMaterial_H

#include "CardiacSolidMechanicsMaterial.h"

//Forward Declarations
class CardiacLinearIsotropicMaterial;
class SymmElasticityTensor;

template<>
InputParameters validParams<CardiacLinearIsotropicMaterial>();

/**
 * LinearIsotropic material for use in simple applications that don't need material properties.
 */
class CardiacLinearIsotropicMaterial : public CardiacSolidMechanicsMaterial
{
public:
  CardiacLinearIsotropicMaterial(const std::string & name,
                          InputParameters parameters);

  virtual ~CardiacLinearIsotropicMaterial();

protected:
  virtual void computeProperties();

  virtual void computeStress(const SymmTensor & strain,
                             SymmTensor & stress);

  Real _youngs_modulus;
  Real _poissons_ratio;

  SymmElasticityTensor * _local_elasticity_tensor;

  Real _pi;
  Real _tol;

};

#endif //CardiacLinearIsotropicMaterial_H
