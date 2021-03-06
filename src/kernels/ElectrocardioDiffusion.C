#include "ElectrocardioDiffusion.h"


template<>
InputParameters validParams<ElectrocardioDiffusion>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<Real>("surface_to_volume", 1000.0, "Surface-to-volume ratio in 1/cm. Default: 1000.0");
  params.addParam<Real>("capacitance", 1.0, "Capacitance of membrane in muF/cm^2. Default: 1.0");
  return params;
}


ElectrocardioDiffusion::ElectrocardioDiffusion(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _surface_to_volume(getParam<Real>("surface_to_volume")),
    _capacitance(getParam<Real>("capacitance")),
    _conductivity(getMaterialProperty<RealTensorValue>("conductivity"))
{
}

ElectrocardioDiffusion::~ElectrocardioDiffusion()
{
}

Real
ElectrocardioDiffusion::computeQpResidual()
{
  // Note: The factor 1000 comes in from unit conversion, cf. docu of #ElectrocardioForcing.h
  return 1000.0*(1.0/_surface_to_volume)*(1.0/_capacitance)*(_grad_test[_i][_qp] * (_conductivity[_qp]*_grad_u[_qp]));
}

Real
ElectrocardioDiffusion::computeQpJacobian()
{
  return 1000.0*(1.0/_surface_to_volume)*(1.0/_capacitance)*(_grad_test[_i][_qp] * (_conductivity[_qp]*_grad_phi[_j][_qp]));
}
