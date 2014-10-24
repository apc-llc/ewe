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
    InitialCondition(name, parameters),
    _activated_cells(0)
{
  std::cout << "Instantiating IC element ... " << std::endl;
}

ElectrocardioIC::~ElectrocardioIC()
{
  std::cout << "Deconstructing IC element ... activated cells = " << _activated_cells << std::endl;
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
  
  const double x_c = 0.5, y_c = 0.5, z_c = 0.5, sig = 0.15;
  
  double arg   = ((p(0) - x_c)/sig)*((p(0) - x_c)/sig) + ((p(1) - y_c)/sig)*((p(1) - y_c)/sig); // + ((p(2) - z_c)/sig)*((p(2) - z_c)/sig);
  double coeff = exp(0.0);
  //return (1/coeff)*exp(-0.5*arg);
  //return -90.272 + 60.0*(1/coeff)*exp(-0.5*arg);
  return -60.0;
  
  //return sin(M_PI*p(0))*sin(M_PI*p(1));
  
  
  /**
  const double x_c = 2.23380e6 , y_c = 8.48238e6 , z_c = 5.89848e6;
  const double dx = 1.078e7, dy = 1.443e7, dz = 1.43e7;
  
  double dist = sqrt( pow( ((p(0)-x_c)/dx), 2) + pow( ((p(1) - y_c)/dy), 2) + pow( ((p(2) - z_c)/dz), 2));
  
 
  if (dist<0.04)
  {
    std::cout << "Cell stimulated - distance = " << dist << std::endl;
    
    _activated_cells++;
    return -10.0;
  }
  else
  {
    Membrane_cell_info cell_info;  // TODO: this should just be a pointer to CardiacPropertiesMaterial::_cell_info[_qp]; unfortunately, we do not have easy access to it. would have to add MaterialPropertyInterface to the ElectrocardioIC class (see eg KernelBase), but we presumably do not have a valid _qp here...
    cell_info.mcode = 1;           // membrane model code -- BERNUS
    cell_info.ccode = BERNUS_EPIC; // membrane model-specific cell type code --> BERNUS_EPIC
    cell_info.param = NULL;        // invalid to avoid accidential use
  
    double vmem;
    ion_init_potential_qp(0.0, cell_info, &vmem);
    return vmem;
  }
   */

}
