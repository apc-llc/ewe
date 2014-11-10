/****************************************************************/
/*                                                              */
/****************************************************************/

#ifndef CardiacThicknessParameterAux_H
#define CardiacThicknessParameterAux_H

#include "AuxKernel.h"

class CardiacThicknessParameterAux;

template<>
InputParameters validParams<CardiacThicknessParameterAux>();

class CardiacThicknessParameterAux : public AuxKernel
{
public:

  CardiacThicknessParameterAux(const std::string & name, InputParameters parameters);

  virtual ~CardiacThicknessParameterAux() {}

protected:
  virtual Real computeValue();

  const VariableValue & _d_rv, & _d_lv, & _d_o;

};

#endif //CardiacThicknessParameterAux_H
