/****************************************************************/
/*                                                              */
/****************************************************************/

#include "ElectrocardioDiffusion.h"


template<>
InputParameters validParams<ElectrocardioDiffusion>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<Real>("surface_to_volume", 1000.0, "Surface-to-volume ratio");
  params.addParam<Real>("capacitance", 1.0, "Capacitance of membrane");
  return params;
}


ElectrocardioDiffusion::ElectrocardioDiffusion(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _surface_to_volume(getParam<Real>("surface_to_volume")),
    _conductivity(getMaterialProperty<Real>("conductivity")),
    _capacitance(getParam<Real>("capacitance"))
{
  std::cout << "Instantiating ElectrocardioDiffusion ... surface_to_volume = " << _surface_to_volume << std::endl;
  std::cout << "Instantiating ElectrocardioDiffusion ... capacitance = " << _capacitance << std::endl;

}

ElectrocardioDiffusion::~ElectrocardioDiffusion()
{
  std::cout << "Destructing ElectrocardioDiffusion ... " << std::endl;
}

Real
ElectrocardioDiffusion::computeQpResidual()
{
  // @todo TODO: actually, we should use a direction-dependent conductivity here, see setup_substances() in anatomy.c for examples for these
  // To this end, prm_substance[n].sigma_XYZ contain the conductivities
  // see CardiacPropertiesMaterial - from there we should fetch the direction-dependent conductivity tensor
  
  // Note: The factor 1000 comes in from unit conversion, cf. docu of #ElectrocardioForcing.h
  return 1000.0*(1.0/_surface_to_volume)*(1.0/_capacitance)*_conductivity[_qp]*(_grad_test[_i][_qp] * _grad_u[_qp]);
}

Real
ElectrocardioDiffusion::computeQpJacobian()
{
  return 1000.0*(1.0/_surface_to_volume)*(1.0/_capacitance)*_conductivity[_qp]*(_grad_test[_i][_qp] * _grad_phi[_j][_qp]);
}
