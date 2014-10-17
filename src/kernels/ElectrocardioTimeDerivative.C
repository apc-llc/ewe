/****************************************************************/
/*                                                              */
/****************************************************************/

#include "ElectrocardioTimeDerivative.h"

#include "Material.h"

template<>
InputParameters validParams<ElectrocardioTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  params.addParam<Real>("capacitance", 1.0, "Capacitance of membrane");
  return params;
}

ElectrocardioTimeDerivative::ElectrocardioTimeDerivative(const std::string & name,
                                             InputParameters parameters) :
    TimeDerivative(name,parameters),
    // This kernel expects an input parameter named "capacitance"
    _capacitance(getParam<Real>("capacitance"))
{
  std::cout << "Instantiating ElectrocardioTimeDerivative .... capacitance = " << _capacitance << std::endl;
}

Real
ElectrocardioTimeDerivative::computeQpResidual()
{
  return _capacitance*TimeDerivative::computeQpResidual();
}

Real
ElectrocardioTimeDerivative::computeQpJacobian()
{
  return _capacitance*TimeDerivative::computeQpJacobian();
}
