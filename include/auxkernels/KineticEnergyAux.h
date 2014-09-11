/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef KINETICENERGYAUX_H
#define KINETICENERGYAUX_H

#include "AuxKernel.h"


//Forward Declarations
class KineticEnergyAux;

template<>
InputParameters validParams<KineticEnergyAux>();

/**
 * Coupled auxiliary value
 */
class KineticEnergyAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  KineticEnergyAux(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeValue();

  std::vector<std::string> _str_append;
  Real _density;
};

#endif //KINETICENERGYAUX_H
