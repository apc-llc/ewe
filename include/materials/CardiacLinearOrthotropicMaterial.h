#ifndef CardiacLinearOrthotropicMaterial_H
#define CardiacLinearOrthotropicMaterial_H

#include "CardiacSolidMechanicsMaterial.h"

//Forward Declarations
class CardiacLinearOrthotropicMaterial;
class SymmElasticityTensor;

template<>
InputParameters validParams<CardiacLinearOrthotropicMaterial>();

/**
 * Material for providing different lienar elastic properties in the three
 * cartesian directions (i.e. linear orthotrophy).
 * The Material is a generalization of LinearIsotropicMaterial.
 * Local coordinate directions are fetched via the respective properties
 * of a CardiacFibresMaterial.
 */
class CardiacLinearOrthotropicMaterial : public CardiacSolidMechanicsMaterial
{
public:
  CardiacLinearOrthotropicMaterial(const std::string & name,
                          InputParameters parameters);

  virtual ~CardiacLinearOrthotropicMaterial();

protected:
  virtual void computeProperties();

  std::vector<Real> _youngs_moduli;   ///< Young's moduli for the three local coordinate directions
  std::vector<Real> _poissons_ratios; ///< Poisson's ratios for the three local coordinate directions
  std::vector<Real> _shear_moduli;    ///< Shear moduli for the three local coordinate directions
  
  MaterialProperty<RealTensorValue> & _Rf;

  SymmElasticityTensor * _local_elasticity_tensor;

  Real _pi;
  Real _tol;

};

#endif //CardiacLinearOrthotropicMaterial_H
