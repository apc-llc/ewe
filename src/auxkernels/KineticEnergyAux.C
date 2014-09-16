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
  params.addCoupledVar("disp_x", "The x displacement");
  params.addCoupledVar("disp_y", "The y displacement");
  params.addCoupledVar("disp_z", "The z displacement");
  //params.set<bool>("use_displaced_mesh") = true;
  return params;
}

KineticEnergyAux::KineticEnergyAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
    _density(getParam<Real>("density")),
    _has_x(isCoupled("disp_x")),
    _has_y(isCoupled("disp_y")),
    _has_z(isCoupled("disp_z")),
    _disp_x(_has_x ? coupledValue("disp_x") : _zero),
    _disp_y(_has_y ? coupledValue("disp_y") : _zero),
    _disp_z(_has_z ? coupledValue("disp_z") : _zero),
    _disp_x_old(_has_x ? coupledValueOld("disp_x") : _zero),
    _disp_y_old(_has_y ? coupledValueOld("disp_y") : _zero),
    _disp_z_old(_has_z ? coupledValueOld("disp_z") : _zero)
{}

Real
KineticEnergyAux::computeValue()
{
  Real velsq = 0.;
  
  if (_has_x) { Real dot_disp = (_disp_x[_qp]-_disp_x_old[_qp])/_dt; velsq += dot_disp*dot_disp; }
  if (_has_y) { Real dot_disp = (_disp_y[_qp]-_disp_y_old[_qp])/_dt; velsq += dot_disp*dot_disp; }
  if (_has_z) { Real dot_disp = (_disp_z[_qp]-_disp_z_old[_qp])/_dt; velsq += dot_disp*dot_disp; }
  
  return _density/2. * velsq;
}
