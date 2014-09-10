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
  params.addRequiredCoupledVar("disp_x", "x-displacement");
  params.addRequiredCoupledVar("disp_y", "y-displacement");
  params.addRequiredCoupledVar("disp_z", "z-displacement");
  return params;
}

KineticEnergyAux::KineticEnergyAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),

    _disp_x(coupledValue("disp_x")),
    _disp_x_old(coupledValueOld("disp_x")),
    _disp_y(coupledValue("disp_y")),
    _disp_y_old(coupledValueOld("disp_y")),
    _disp_z(coupledValue("disp_z")),
    _disp_z_old(coupledValueOld("disp_z")),
    _density(getParam<Real>("density"))
{}

Real
KineticEnergyAux::computeValue()
{
  Point vel = Point(_disp_x[_qp]-_disp_x_old[_qp],
                    _disp_y[_qp]-_disp_y_old[_qp],
	            _disp_z[_qp]-_disp_z_old[_qp]) / _dt;
  return _density/2. * (vel*vel);
}
