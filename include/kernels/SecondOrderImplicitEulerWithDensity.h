/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef SECONDORDERIMPLICITEULERWITHDENSITY_H
#define SECONDORDERIMPLICITEULERWITHDENSITY_H

#include "SecondDerivativeImplicitEuler.h"

// Forward Declarations
class SecondOrderImplicitEulerWithDensity;

template<>
InputParameters validParams<SecondOrderImplicitEulerWithDensity>();

class SecondOrderImplicitEulerWithDensity : public SecondDerivativeImplicitEuler
{
public:

  SecondOrderImplicitEulerWithDensity(const std::string & name,
                        InputParameters parameters);


  virtual void computeJacobian();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  Real _density;
  bool _lumping;
};

#endif //SECONDORDERIMPLICITEULERWITHDENSITY_H
