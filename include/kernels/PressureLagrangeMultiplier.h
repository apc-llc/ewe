/*************************************
 * Template for this file came from moose/test/include/kernels/ScalarLagrangeMultiplier.h
 *************************************/
#ifndef PressureLagrangeMultiplier_H
#define PressureLagrangeMultiplier_H

#include "Kernel.h"

class PressureLagrangeMultiplier;

template<>
InputParameters validParams<PressureLagrangeMultiplier>();

/**
 * Implements coupling of Lagrange multiplier (as a scalar variable) to a simple constraint of type (g(u) - c = 0)
 */
class PressureLagrangeMultiplier : public Kernel
{
public:
  PressureLagrangeMultiplier(const std::string & name, InputParameters parameters);

  virtual void computeOffDiagJacobianScalar(unsigned int jvar);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  MaterialProperty<Real> & _J;
};

#endif /* PressureLagrangeMultiplier_H */
