#include "KineticEnergyNewmarkAux.h"

template<>
InputParameters validParams<KineticEnergyNewmarkAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("density", 1.0, "Mass density");
  params.addRequiredCoupledVar("displacements", "A list of displacement variables");
  //params.set<bool>("use_displaced_mesh") = true;
  return params;
}

KineticEnergyNewmarkAux::KineticEnergyNewmarkAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
    _density(getParam<Real>("density"))
{
  // see http://mooseframework.org/wiki/Faq/#coupling-to-an-arbitrary-number-of-variables-back-to-top for details on this magic
  _vel.resize(coupledComponents("displacements"));

  for (unsigned int i=0; i<_vel.size(); ++i) {
    _vel[i] = &(getMaterialProperty<Real>("newmark_velocity-" + (getVar("displacements", i))->name()));
  }
}

Real
KineticEnergyNewmarkAux::computeValue()
{
  Real velsq(0);

  for (unsigned int i=0;i<_vel.size();i++)
    velsq += (*_vel[i])[_qp] * (*_vel[i])[_qp];

  return _density/2. * velsq;
}
