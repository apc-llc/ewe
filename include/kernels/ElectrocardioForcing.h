/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef ELECTROCARDIOFORCING_H
#define ELECTROCARDIOFORCING_H

#include "Kernel.h"

class ElectrocardioForcing;

template<>
InputParameters validParams<ElectrocardioForcing>();


class ElectrocardioForcing : public Kernel
{
public:
  ElectrocardioForcing(const std::string & name, InputParameters parameters);
  virtual ~ElectrocardioForcing();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:

  double _ion_coeff;
  MaterialProperty<Real> & _Iion;
  
};
  
#endif /* ELECTROCARDIOFORCING_H */
