/*************************************
 * Template for this file came from moose/modules/solid_mechanics/include/materials/total_strain/SolidMechanicsMaterial.h
 *************************************/


#ifndef CardiacSolidMechanicsMaterial_H
#define CardiacSolidMechanicsMaterial_H

#include "Material.h"
#include "SymmElasticityTensor.h"

//Forward Declarations
class CardiacSolidMechanicsMaterial;
class VolumetricModel;

template<>
InputParameters validParams<CardiacSolidMechanicsMaterial>();

/**
 * SolidMechanics material for use in simple applications that don't need material properties.
 */
class CardiacSolidMechanicsMaterial : public Material
{
public:
  CardiacSolidMechanicsMaterial(const std::string & name, InputParameters parameters);

protected:
  std::vector<VariableGradient *> _grad_disp;

  bool _has_c;
  VariableValue & _c;

  MaterialProperty<SymmTensor> & _stress;
  MaterialProperty<SymmElasticityTensor> & _elasticity_tensor;
  MaterialProperty<SymmElasticityTensor> & _Jacobian_mult;

  MaterialProperty<SymmTensor> & _elastic_strain;
};

#endif //CardiacSolidMechanicsMaterial_H
