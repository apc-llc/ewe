/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "KineticEnergyAux.h"

template<>
InputParameters validParams<KineticEnergyAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("density", 1.0, "Mass density");
  params.addRequiredParam<std::vector<std::string> >("str_append", "Strings that have been appended to material property names (connection to NewmarkMaterial)");
  return params;
}

KineticEnergyAux::KineticEnergyAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),

    _str_append(getParam<std::vector<std::string> >("str_append")),
    _density(getParam<Real>("density"))
{}

Real
KineticEnergyAux::computeValue()
{
  Real velsq = 0.;
  
  for(std::vector<std::string>::iterator it = _str_append.begin(); it != _str_append.end(); ++it) {
    Real vel = getMaterialProperty<Real>("newmark_velocity" + *it)[_qp];
    velsq += vel*vel;
  }
  
  return _density/2. * velsq;
}
