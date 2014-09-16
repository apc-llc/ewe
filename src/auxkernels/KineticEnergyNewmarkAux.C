/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "KineticEnergyNewmarkAux.h"

template<>
InputParameters validParams<KineticEnergyNewmarkAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("density", 1.0, "Mass density");
  params.addCoupledVar("disp_x", "The x displacement");
  params.addCoupledVar("disp_y", "The y displacement");
  params.addCoupledVar("disp_z", "The z displacement");
  //params.set<bool>("use_displaced_mesh") = true;
  return params;
}

KineticEnergyNewmarkAux::KineticEnergyNewmarkAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
    _density(getParam<Real>("density")),
    _has_x(isCoupled("disp_x")),
    _has_y(isCoupled("disp_y")),
    _has_z(isCoupled("disp_z")),
    _vel_x(_has_x ? &(getMaterialProperty<Real>("newmark_velocity-" + (getVar("disp_x", 0))->name())) : NULL),
    _vel_y(_has_y ? &(getMaterialProperty<Real>("newmark_velocity-" + (getVar("disp_y", 0))->name())) : NULL),
    _vel_z(_has_z ? &(getMaterialProperty<Real>("newmark_velocity-" + (getVar("disp_z", 0))->name())) : NULL)
{}

Real
KineticEnergyNewmarkAux::computeValue()
{
  Real velsq = 0.;
  
  if (_has_x) velsq += (*_vel_x)[_qp] * (*_vel_x)[_qp];
  if (_has_y) velsq += (*_vel_y)[_qp] * (*_vel_y)[_qp];
  if (_has_z) velsq += (*_vel_z)[_qp] * (*_vel_z)[_qp];
  
  return _density/2. * velsq;
}
