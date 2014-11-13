#ifndef SECONDORDERIMPLICITEULERWITHDENSITY_H
#define SECONDORDERIMPLICITEULERWITHDENSITY_H

#include "SecondDerivativeImplicitEuler.h"

class SecondOrderImplicitEulerWithDensity;

template<>
InputParameters validParams<SecondOrderImplicitEulerWithDensity>();

/**
 * Second order implicit Euler kernel for PDE terms of the form
 * \f$ -\rho \frac{\partial^2u}{\partial t^2} \f$.
 *
 * This kernel just uses SecondDerivativeImplicitEuler multiplied
 * by \f$ -\rho \f$.
 * Furthermore, it supports mass matrix lumping. The idea for that
 * specific feature came from the TimeDerivative class.
 */
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
