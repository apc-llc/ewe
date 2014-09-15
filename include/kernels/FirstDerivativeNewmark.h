/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef FIRSTDERIVATIVENEWMARK_H
#define FIRSTDERIVATIVENEWMARK_H

#include "TimeKernel.h"
#include "Material.h"

class FirstDerivativeNewmark;

template<>
InputParameters validParams<FirstDerivativeNewmark>();

class FirstDerivativeNewmark : public TimeKernel
{
public:
  FirstDerivativeNewmark(const std::string & name, InputParameters parameters);

  virtual void computeJacobian();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:

  std::string _comp_name;

  Real _friction;
  bool _lumping;

  MaterialProperty<Real> & _acc;
  MaterialProperty<Real> & _vel;
  MaterialProperty<Real> & _gamma;
  MaterialProperty<Real> & _beta;
};

#endif //FIRSTDERIVATIVENEWMARK_H
