/****************************************************************/
/*                                                              */
/****************************************************************/

#include "ElectrocardioTimeDerivative.h"

#include "Material.h"

template<>
InputParameters validParams<ElectrocardioTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  return params;
}

ElectrocardioTimeDerivative::ElectrocardioTimeDerivative(const std::string & name,
                                             InputParameters parameters) :
    TimeDerivative(name,parameters)
{
}

Real
ElectrocardioTimeDerivative::computeQpResidual()
{
  return TimeDerivative::computeQpResidual();
}

Real
ElectrocardioTimeDerivative::computeQpJacobian()
{
  return TimeDerivative::computeQpJacobian();
}
