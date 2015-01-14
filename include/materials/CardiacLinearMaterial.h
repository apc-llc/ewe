#ifndef CardiacLinearMaterial_H
#define CardiacLinearMaterial_H

#include "CardiacMechanicsMaterial.h"
#include "SymmTensor.h"

class CardiacLinearMaterial;

template<>
InputParameters validParams<CardiacLinearMaterial>();

/**
 * @todo: documentation
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

  const Real _lambda, _mu; ///< Lame constants
};

#endif //CardiacLinearMaterial_H
