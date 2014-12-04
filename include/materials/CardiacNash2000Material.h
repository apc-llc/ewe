#ifndef CardiacNash2000Material_H
#define CardiacNash2000Material_H

#include "CardiacMEchanicsMaterial.h"
#include "SymmTensor.h"

class CardiacNash2000Material;

template<>
InputParameters validParams<CardiacNash2000Material>();

/**
 * Implementation of the 'pole-free' hyperelastic material law
 * for cardiac tissue as given in [Nash 2000].
 *
 * Parameters are read via MaterialParameters from the
 * input file. A linear material behavior (for debugging)
 * can be realized by setting the approriate \f$k_{ij}\f$ to
 * negative values, see CardiacNash2000Material::computeQpProperties()
 * for details.
 */
class CardiacNash2000Material : public CardiacMechanicsMaterial
{
public:
  CardiacNash2000Material(const std::string & name,
                          InputParameters parameters);

protected:
  virtual void computeQpStressProperties(const SymmTensor &E);

  SymmTensor _k, _a, _b;
};

#endif //CardiacNash2000Material_H
