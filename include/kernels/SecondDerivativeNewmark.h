/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef SECONDDERIVATIVENEWMARK_H
#define SECONDDERIVATIVENEWMARK_H

#include "TimeKernel.h"
#include "Material.h"

class SecondDerivativeNewmark;

template<>
InputParameters validParams<SecondDerivativeNewmark>();

class SecondDerivativeNewmark : public TimeKernel
{
public:
  SecondDerivativeNewmark(const std::string & name, InputParameters parameters);

  virtual void computeJacobian();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  Real _density;
  bool _lumping;
  MooseEnum _c;
  
  MaterialProperty<Point> & _delta_a;
  MaterialProperty<Real> & _beta;
};

#endif //SECONDDERIVATIVENEWMARK_H
