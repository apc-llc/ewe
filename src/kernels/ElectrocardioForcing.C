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
  
  double x(_q_point[_qp](0));
  double y(_q_point[_qp](1));
  double z(_q_point[_qp](2));
  double time(_t);

  // Externally applied current
  //Real _Iion_app( 10.0*pow(cos(M_PI*(x-time)), 6.0) );
  Real _Iion_app( -1500*exp(-0.5*(x*x/0.01) - 0.5*(y*y/0.01)));
  if (time>2.0) _Iion_app = 0.0;
  
  return _ion_coeff*(_Iion[_qp] - _Iion_app)*_test[_i][_qp];
}

Real
ElectrocardioForcing::computeQpJacobian()
{
  return 0.0; 
}