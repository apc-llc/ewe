/****************************************************************/
/*                                                              */
/****************************************************************/

#include "ElectrocardioIC.h"
#include <math.h>

template<>
InputParameters validParams<ElectrocardioIC>()
{
  InputParameters params = validParams<InitialCondition>();
  return params;
}

ElectrocardioIC::ElectrocardioIC(const std::string & name,
                     InputParameters parameters) :
    InitialCondition(name, parameters)
{
  std::cout << "Instantiating IC element ... " << std::endl;
}

ElectrocardioIC::~ElectrocardioIC()
{
  std::cout << "Deconstructing IC element ..." << std::endl;
}

Real
ElectrocardioIC::value(const Point & p)
{
  /**
   * _value * x
   * The Point class is defined in libMesh.  The spatial
   * coordinates x,y,z can be accessed individually using
   * the parenthesis operator and a numeric index from 0..2
   */
  // return 2.*std::abs(p(0));
  
  // The value -90.272 is the resting potential of the bernus model for all except one case...
  
  const double x_c = 0.5, y_c = 0.5, sig = 0.15;
  
  double arg   = ((p(0) - x_c)/sig)*((p(0) - x_c)/sig) + ((p(1) - y_c)/sig)*((p(1) - y_c)/sig); // + ((p(2) - z_c)/sig)*((p(2) - z_c)/sig);
  double coeff = exp(0.0);
  //return (1/coeff)*exp(-0.5*arg);
  //return -90.272 + 30.0*(1/coeff)*exp(-0.5*arg);
  //return -60.0;
  return -90.272;
  //return sin(M_PI*p(0))*sin(M_PI*p(1));

}
