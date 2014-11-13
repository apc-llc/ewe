#include "AnisotropicGradientShift.h"


template<>
InputParameters validParams<AnisotropicGradientShift>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<RealTensorValue>("tensor_coeff", RealTensorValue(1,0,0,0,1,0,0,0,1), "The Tensor to multiply the Diffusion operator by. Default: identity.");
  params.addRequiredParam<unsigned int>("component", "An integer corresponding to the direction the variable this kernel acts in. (0 for x, 1 for y, 2 for z)");
  return params;
}


AnisotropicGradientShift::AnisotropicGradientShift(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _k(getParam<RealTensorValue>("tensor_coeff")),
   _component(getParam<unsigned int>("component"))
{
}

Real
AnisotropicGradientShift::computeQpResidual()
{
  Real res(0);

  for (unsigned int M=0;M<3;M++)
    res += _k(M,_component) * _grad_test[_i][_qp](M);

  return res;
}

Real
AnisotropicGradientShift::computeQpJacobian()
{
  return 0;
}
