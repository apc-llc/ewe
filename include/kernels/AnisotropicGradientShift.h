/****************************************************************/
/*                                                              */
/****************************************************************/

#ifndef AnisotropicGradientShift_H
#define AnisotropicGradientShift_H

#include "Kernel.h"

class AnisotropicGradientShift;

template<>
InputParameters validParams<AnisotropicGradientShift>();


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
