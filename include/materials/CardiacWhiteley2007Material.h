/*************************************
 * Template for this file came from moose/modules/solid_mechanics/include/materials/total_strain/LinearIsotropicMaterial.h
 *************************************/

#ifndef CardiacWhiteley2007Material_H
#define CardiacWhiteley2007Material_H

#include "CardiacSolidMechanicsMaterial.h"

//Forward Declarations
class CardiacWhiteley2007Material;
class SymmElasticityTensor;

template<>
InputParameters validParams<CardiacWhiteley2007Material>();

/**
 * LinearOrthotropic material for use in simple applications that don't need material properties.
 */
class CardiacWhiteley2007Material : public CardiacSolidMechanicsMaterial
{
public:
  CardiacWhiteley2007Material(const std::string & name,
                          InputParameters parameters);

  virtual ~CardiacWhiteley2007Material();

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

#endif //CardiacWhiteley2007Material_H
