/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "AccVelStoringMaterial.h"

template<>
InputParameters validParams<AccVelStoringMaterial>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredParam<Real>("beta","Newmark integration beta parameter (will also be provided as material property for use by other kernels)");
  params.addRequiredParam<Real>("gamma","Newmark integration gamma parameter (will also be provided as material property for use by other kernels)");

  params.addRequiredParam<Real>("density","Materials mass density (will also be provided as material property for use by other kernels)");

  params.addCoupledVar("disp_x", "Displacement in x direction");
  params.addCoupledVar("disp_y", "Displacement in y direction");
  params.addCoupledVar("disp_z", "Displacement in z direction");

  return params;
}

AccVelStoringMaterial::AccVelStoringMaterial(const std::string & name,
                                 InputParameters parameters) :
    Material(name, parameters),
    _force(declareProperty<Point>("force")),
    _df_du(declareProperty<Point>("dforce_du")),
    _accel_new(declareProperty<Point>("acceleration")),
    _accel_old(declareProperty<Point>("acceleration_old")),
    _vel_new(declareProperty<Point>("velocity")),
    _vel_old(declareProperty<Point>("velocity_old")),
    _disp_x(coupledValue("disp_x")),
    _disp_y(coupledValue("disp_y")),
    _disp_z(coupledValue("disp_z")),
    _disp_x_old(coupledValueOld("disp_x")),
    _disp_y_old(coupledValueOld("disp_y")),
    _disp_z_old(coupledValueOld("disp_z")),
    _beta(getParam<Real>("beta")),
    _gamma(getParam<Real>("gamma")),
    _density(getParam<Real>("density"))
{}

void
AccVelStoringMaterial::computeQpProperties()
{
  // The following is the Newmark method that we use to determine new velocity and accelerations
  
  // FIXME: what about the very first timestep ?? no _old variables will have valid values
  Point delta_disp = Point(_disp_x[_qp]-_disp_x_old[_qp],
                           _disp_y[_qp]-_disp_y_old[_qp],
			   _disp_z[_qp]-_disp_z_old[_qp]);
  _vel_old[_qp]   = _vel_new[_qp];
  _accel_old[_qp] = _accel_new[_qp];
  _accel_new[_qp] = ((delta_disp/(_dt*_dt)) - _vel_old[_qp]/_dt - _accel_old[_qp]*(0.5-_beta)) / _beta;
  _vel_new[_qp]   = _vel_old[_qp] + _dt * ( (1.-_gamma)*_accel_old[_qp] + _gamma*_accel_new[_qp] );
  
  _force[_qp] = _density * _accel_new[_qp];
  
  // FIXME: is the minus actually right? - dont we derive by new position instead of the old?
  Real df_du = _density/(-_beta*_dt*_dt);
  _df_du[_qp] = Point(df_du, df_du, df_du);
  
  Moose::out << _qp << " vel = " << _vel_new[_qp] << ", accel = " << _accel_new[_qp] << ", force = " << _force[_qp] << " " << df_du << std::endl;
}
