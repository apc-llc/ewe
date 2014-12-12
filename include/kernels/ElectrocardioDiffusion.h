#ifndef ELECTROCARDIODIFFUSION_H
#define ELECTROCARDIODIFFUSION_H

#include "Kernel.h"
#include "libmesh/tensor_value.h"

class ElectrocardioDiffusion;

template<>
InputParameters validParams<ElectrocardioDiffusion>();


class ElectrocardioDiffusion : public Kernel
{
public:
  ElectrocardioDiffusion(const std::string & name, InputParameters parameters);
  virtual ~ElectrocardioDiffusion();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  
private:
  Real _surface_to_volume;
  Real _capacitance;
  MaterialProperty<RealTensorValue> & _conductivity;
};


#endif /* ELECTROCARDIODIFFUSION_H */
