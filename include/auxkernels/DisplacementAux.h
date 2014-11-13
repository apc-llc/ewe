/****************************************************************/
/*                                                              */
/****************************************************************/

#ifndef DisplacementAux_H
#define DisplacementAux_H

#include "AuxKernel.h"

class DisplacementAux;

template<>
InputParameters validParams<DisplacementAux>();

class DisplacementAux : public AuxKernel
{
public:

  DisplacementAux(const std::string & name, InputParameters parameters);

  virtual ~DisplacementAux() {}

protected:
  virtual Real computeValue();

  const unsigned int & _component;
  const VariableValue & _coordinate;

};

#endif //DisplacementAux_H
