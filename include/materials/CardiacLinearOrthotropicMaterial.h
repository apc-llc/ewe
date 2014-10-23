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

  std::vector<Real> _youngs_moduli;
  std::vector<Real> _poissons_ratios;
  std::vector<Real> _shear_moduli;
  
  MaterialProperty<RealTensorValue> & _Rf;

  SymmElasticityTensor * _local_elasticity_tensor;

  Real _pi;
  Real _tol;

};

#endif //CardiacLinearOrthotropicMaterial_H
