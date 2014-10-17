/****************************************************************/
/*                                                              */
/****************************************************************/

#ifndef ELECTROCARDIODIFFUSION_H
#define ELECTROCARDIODIFFUSION_H

#include "Kernel.h"

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
  double _surface_to_volume;
  MaterialProperty<Real> & _conductivity;
};


#endif /* ELECTROCARDIODIFFUSION_H */
