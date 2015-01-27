#ifndef KINETICENERGYNEWMARKAUX_H
#define KINETICENERGYNEWMARKAUX_H

#include "AuxKernel.h"


class KineticEnergyNewmarkAux;

template<>
InputParameters validParams<KineticEnergyNewmarkAux>();

/**
 * This kernels computes the kinetic energy
 * using the velocity components from the respective
 * NewmarkMaterial instance:
 * \f$E_{\rm kin}=\frac{\varrho}{2}v^2\f$.
 */
class KineticEnergyNewmarkAux : public AuxKernel
{
public:

  KineticEnergyNewmarkAux(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeValue();
  
  Real _density;
  std::vector<MaterialProperty<Real> *> _vel;
};

#endif //KINETICENERGYNEWMARKAUX_H
