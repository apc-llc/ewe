/*************************************
 * Template for this file came from moose/modules/solid_mechanics/include/materials/total_strain/LinearIsotropicMaterial.h
 *************************************/

#ifndef CardiacLinearOrthotropicMaterial_H
#define CardiacLinearOrthotropicMaterial_H

#include "CardiacSolidMechanicsMaterial.h"

//Forward Declarations
class CardiacLinearOrthotropicMaterial;
class SymmElasticityTensor;

template<>
InputParameters validParams<CardiacLinearOrthotropicMaterial>();

/**
 * LinearOrthotropic material for use in simple applications that don't need material properties.
 */
class CardiacLinearOrthotropicMaterial : public CardiacSolidMechanicsMaterial
{
public:
  CardiacLinearOrthotropicMaterial(const std::string & name,
                          InputParameters parameters);

  virtual ~CardiacLinearOrthotropicMaterial();

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

#endif //CardiacLinearOrthotropicMaterial_H
