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
  
  // FIXME: I assume, it will be much more efficient if material only works with an individual component
  // instead of computing all three and selecting the appropriate afterwards... 
  MaterialProperty<Point> & _acc;
  MaterialProperty<Point> & _jacobian;
};

#endif //SECONDDERIVATIVENEWMARK_H
