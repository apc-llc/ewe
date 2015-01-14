#ifndef CardiacLinearMaterial_H
#define CardiacLinearMaterial_H

#include "CardiacMechanicsMaterial.h"
#include "SymmTensor.h"

class CardiacLinearMaterial;

template<>
InputParameters validParams<CardiacLinearMaterial>();

/**
 * This Material implements Linear Isotropic Elasticity.
 *
 * Parameters are read via MaterialParameters from the
 * input file.
 */
class CardiacLinearMaterial : public CardiacMechanicsMaterial
{
public:
  CardiacLinearMaterial(const std::string & name,
                          InputParameters parameters);

protected:
  virtual void computeQpStressProperties(const SymmTensor &C, const SymmTensor &E);

  Real _lambda; ///< 1st Lame constant
  Real _mu; ///< 2nd Lame constant
  Real _E; ///< Youngs modulus
  Real _nu; ///< Poissons ratio
};

#endif //CardiacLinearMaterial_H
