#ifndef NeighborElementAverageAux_H
#define NeighborElementAverageAux_H

#include "AuxKernel.h"

class NeighborElementAverageAux;

template<>
InputParameters validParams<NeighborElementAverageAux>();

/**
 * AuxKernel for computing the average of a variable over an elements the neighbor elements.
 */
class NeighborElementAverageAux : public AuxKernel
{
public:

  NeighborElementAverageAux(const std::string & name, InputParameters parameters);

  virtual ~NeighborElementAverageAux() {}

protected:
  virtual Real computeValue();

  const VariableValue & _in;
  MooseVariable const * _in_varptr;
  const Real _weight_centre, _weight_neighbor;
  const bool _volume_weighting;
};

#endif //NeighborElementAverageAux_H
