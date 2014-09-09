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

  VariableValue & _disp_x, & _disp_x_old;
  VariableValue & _disp_y, & _disp_y_old;
  VariableValue & _disp_z, & _disp_z_old;

  Real _density;
};

#endif //KINETICENERGYAUX_H
