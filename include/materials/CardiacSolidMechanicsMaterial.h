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
  const std::string _appended_property_name;
  VariableGradient & _grad_disp_x;
  VariableGradient & _grad_disp_y;
  VariableGradient & _grad_disp_z;

  bool _has_c;
  VariableValue & _c;

  MaterialProperty<SymmTensor> & _stress;
  MaterialProperty<SymmElasticityTensor> & _elasticity_tensor;
  MaterialProperty<SymmElasticityTensor> & _Jacobian_mult;

  MaterialProperty<SymmTensor> & _elastic_strain;

  template<typename T>
  MaterialProperty<T> & createProperty(const std::string & prop_name)
    {
      std::string name(prop_name + _appended_property_name);
      return declareProperty<T>(name);
    }

  template<typename T>
  MaterialProperty<T> & createPropertyOld(const std::string & prop_name)
    {
      std::string name(prop_name + _appended_property_name);
      return declarePropertyOld<T>(name);
    }


};

#endif //CardiacSolidMechanicsMaterial_H
