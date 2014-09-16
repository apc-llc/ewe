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
  
  Real _density;
  bool _has_x, _has_y, _has_z;
  const VariableValue & _disp_x, & _disp_y, &_disp_z;
  const VariableValue & _disp_x_old, & _disp_y_old, & _disp_z_old;
};

#endif //KINETICENERGYAUX_H
