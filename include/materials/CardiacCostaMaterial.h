#ifndef CardiacCostaMaterial_H
#define CardiacCostaMaterial_H

#include "CardiacMechanicsMaterial.h"
#include "SymmTensor.h"

class CardiacCostaMaterial;

template<>
InputParameters validParams<CardiacCostaMaterial>();

/**
 * @todo: documentation
 *
 * Parameters are read via MaterialParameters from the
 * input file.
 */
class CardiacCostaMaterial : public CardiacMechanicsMaterial
{
public:
  CardiacCostaMaterial(const std::string & name,
                          InputParameters parameters);

protected:
  virtual void computeQpStressProperties(const SymmTensor &C, const SymmTensor &E);

  enum CardiacCostaMaterialParameters {C=0, bf, bt, bfs};

  const std::vector<Real> & _p; ///< material parameters, indexed via #CardiacCostaMaterialParameters
  const Real _maxQ;
  const SymmTensor _b;
};

#endif //CardiacCostaMaterial_H
