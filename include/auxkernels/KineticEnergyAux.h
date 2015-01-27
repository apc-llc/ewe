#ifndef KINETICENERGYAUX_H
#define KINETICENERGYAUX_H

#include "AuxKernel.h"


class KineticEnergyAux;

template<>
InputParameters validParams<KineticEnergyAux>();
/**
 * This kernels computes the kinetic energy
 * by simple finite differencing of displacements at
 * subsequent instances in time:
 * \f$E_{\rm kin}=\frac{\varrho}{2}\left(\frac{\Delta x}{\Delta t}\right)^2\f$.
 */
class KineticEnergyAux : public AuxKernel
{
public:

  KineticEnergyAux(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeValue();
  
  Real _density;
  std::vector<VariableValue *> _disp;
  std::vector<VariableValue *> _disp_old;
};

#endif //KINETICENERGYAUX_H
