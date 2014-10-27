/****************************************************************/
/*                                                              */
/****************************************************************/

#include "ElectrocardioForcing.h"
#include <cmath>

template<>
InputParameters validParams<ElectrocardioForcing>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<Real>("ion_coeff", 1.0, "Auxiliary factor before ion current forcing term: Default 1.0, set to zero to deactivate ion current forcing");
  return params;
}


ElectrocardioForcing::ElectrocardioForcing(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _ion_coeff(getParam<Real>("ion_coeff")),
    _Iion(getMaterialProperty<Real>("Iion"))
{
  std::cout << "Instantiating ElectrocardioForcing .... " << std::endl;
  if (_ion_coeff == 0.0)
  {
    std::cout << "DEACTIVATED ion current forcing..." << std::endl;
  }
}

ElectrocardioForcing::~ElectrocardioForcing()
{
  std::cout << "Destructing ElectrocardioForcing .... " << std::endl;
}

Real
ElectrocardioForcing::computeQpResidual()
{
  return _ion_coeff*_Iion[_qp]*_test[_i][_qp];
}

Real
ElectrocardioForcing::computeQpJacobian()
{
  return 0.0; 
}