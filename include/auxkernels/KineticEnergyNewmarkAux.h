/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef KINETICENERGYNEWMARKAUX_H
#define KINETICENERGYNEWMARKAUX_H

#include "AuxKernel.h"


//Forward Declarations
class KineticEnergyNewmarkAux;

template<>
InputParameters validParams<KineticEnergyNewmarkAux>();

/**
 * Coupled auxiliary value
 */
class KineticEnergyNewmarkAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  KineticEnergyNewmarkAux(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeValue();
  
  Real _density;
  bool _has_x, _has_y, _has_z;
  MaterialProperty<Real> *_vel_x, *_vel_y, *_vel_z;
};

#endif //KINETICENERGYNEWMARKAUX_H
