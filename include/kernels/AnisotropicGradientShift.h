#ifndef AnisotropicGradientShift_H
#define AnisotropicGradientShift_H

#include "Kernel.h"

class AnisotropicGradientShift;

template<>
InputParameters validParams<AnisotropicGradientShift>();


/**
 * Kernel for converting between displaced coordinate and displacement interpretation.
 *
 * This kernel is mainly used for testing purposes. It helps comparing results of
 * the standard Diffusion kernel (that works on nonlinear variables \f$x_i\f$)
 * to results from the CardiacKirchhoffStressDivergence kernel that works on
 * displacements \f$u_i=x_i-X_i\f$.
 */
class AnisotropicGradientShift : public Kernel
{
public:
  AnisotropicGradientShift(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  const RealTensorValue & _k;
  const unsigned int & _component;

};


#endif /* AnisotropicGradientShift_H */
