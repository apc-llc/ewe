#ifndef CardiacHolzapfel2009Material_H
#define CardiacHolzapfel2009Material_H

#include "CardiacMechanicsMaterial.h"
#include "SymmTensor.h"

class CardiacHolzapfel2009Material;

template<>
InputParameters validParams<CardiacHolzapfel2009Material>();

/**
 * Implementation of the 'constitutive-modelling' hyperelastic 
 * material for fibre-enforced tissue as given in
 * \ref Holzapfel2009.
 *
 * Parameters are read via MaterialParameters from the
 * input file.
 */
class CardiacHolzapfel2009Material : public CardiacMechanicsMaterial
{
public:
  CardiacHolzapfel2009Material(const std::string & name,
                          InputParameters parameters);

protected:
  virtual void computeQpStressProperties(const SymmTensor &C, const SymmTensor &E);

  enum CardiacHolzapfel2009MaterialParameters {A1=0, B1, Af, Bf, As, Bs, Afs, Bfs};

  const std::vector<Real> & _p; ///< material parameters, indexed via #CardiacHolzapfel2009MaterialParameters
};

#endif //CardiacHolzapfel2009Material_H
