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

  double x_c(0.0);
  double y_c(0.0);
  
  // Externally applied current
  Real _Iion_app(0.0);
  
  if (time<2.0) {
      // Figure 5 in Bernus et al; threshold current for a 5ms stimulus is approximately 5 pA/pF;
      // to excite spiral waves, stimulate with twice the threshold value
    _Iion_app = -20.0*exp(-0.5*pow(x-x_c, 2.0)/pow(0.25, 2.0));
  }
  else if ((time>205.0) && (time<110.0)) {
    //_Iion_app = -20.0*exp(-0.5*pow(x-5.0, 2.0)/pow(0.25, 2.0) - 0.5*pow(y-0.0, 2.0)/pow(1.0, 2.0));
  }
  else {_Iion_app = 0.0; }
  
  return _ion_coeff*(_Iion[_qp] + _Iion_app)*_test[_i][_qp];
}

Real
ElectrocardioForcing::computeQpJacobian()
{
  return 0.0; 
}
