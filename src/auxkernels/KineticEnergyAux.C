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
  params.addRequiredCoupledVar("displacements", "A list of displacement variables");
  //params.set<bool>("use_displaced_mesh") = true;
  return params;
}

KineticEnergyAux::KineticEnergyAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
    _density(getParam<Real>("density"))
{
  // see http://mooseframework.org/wiki/Faq/#coupling-to-an-arbitrary-number-of-variables-back-to-top for details on this magic
  _disp.resize(coupledComponents("displacements"));
  _disp_old.resize(coupledComponents("displacements"));

  for (unsigned int i=0; i<_disp.size(); ++i) {
    _disp[i]     = &coupledValue(   "displacements", i);
    _disp_old[i] = &coupledValueOld("displacements", i);
  }
}

Real
KineticEnergyAux::computeValue()
{
  Real distsq(0);

  for (unsigned int i=0;i<_disp.size();i++) {
    Real dot_disp( (*_disp[i])[_qp] - (*_disp_old[i])[_qp] );
    distsq += dot_disp*dot_disp;
  }

  return _density/(2.*_dt*_dt) * distsq;
}
