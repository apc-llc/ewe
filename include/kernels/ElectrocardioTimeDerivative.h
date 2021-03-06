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

#ifndef ELECTROCARDIOTIMEDERIVATIVE_H
#define ELECTROCARDIOTIMEDERIVATIVE_H

#include "TimeDerivative.h"

// Forward Declarations
class ElectrocardioTimeDerivative;

template<>
InputParameters validParams<ElectrocardioTimeDerivative>();

class ElectrocardioTimeDerivative : public TimeDerivative
{
public:

  ElectrocardioTimeDerivative(const std::string & name,
                        InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

};

#endif //ELECTROCARDIOTIMEDERIVATIVE_H
