#ifndef KINETICENERGYAUX_H
#define KINETICENERGYAUX_H

#include "AuxKernel.h"


//Forward Declarations
class KineticEnergyAux;

template<>
InputParameters validParams<KineticEnergyAux>();

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
