/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef MOMENTUMTRANSIENT_H
#define MOMENTUMTRANSIENT_H

#include "TimeDerivative.h"

// Forward Declarations
class MomentumTransient;

template<>
InputParameters validParams<MomentumTransient>();

class MomentumTransient : public TimeDerivative
{
public:

  MomentumTransient(const std::string & name,
                        InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  /*
   * Mass density
   */
  Real _rho;
};

#endif //MOMENTUMTRANSIENT_H
