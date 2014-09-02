/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef POSITIONTRANSIENT_H
#define POSITIONTRANSIENT_H

#include "TimeDerivative.h"

// Forward Declarations
class PositionTransient;

template<>
InputParameters validParams<PositionTransient>();

class PositionTransient : public TimeDerivative
{
public:

  PositionTransient(const std::string & name,
                        InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();
};

#endif //POSITIONTRANSIENT_H
