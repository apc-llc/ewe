/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "NewmarkMaterial.h"

template<>
InputParameters validParams<NewmarkMaterial>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredParam<Real>("beta","Newmark integration beta parameter (will also be provided as material property for use by other kernels)");
  params.addRequiredParam<Real>("gamma","Newmark integration gamma parameter (will also be provided as material property for use by other kernels)");

  params.addCoupledVar("disp_x", "Displacement in x direction");
  params.addCoupledVar("disp_y", "Displacement in y direction");
  params.addCoupledVar("disp_z", "Displacement in z direction");

  return params;
}

NewmarkMaterial::NewmarkMaterial(const std::string & name,
                                 InputParameters parameters) :
    Material(name, parameters),
    _delta_a(declareProperty<Point>("newmark_delta_a")),
    _vel(declareProperty<Point>("newmark_velocity")),     _vel_old(declarePropertyOld<Point>("newmark_velocity")),
    _acc(declareProperty<Point>("newmark_acceleration")), _acc_old(declarePropertyOld<Point>("newmark_acceleration")),
    _jacobian(declareProperty<Point>("newmark_jacobian")),
    _disp_x(coupledValue("disp_x")), _disp_x_old(coupledValueOld("disp_x")),
    _disp_y(coupledValue("disp_y")), _disp_y_old(coupledValueOld("disp_y")),
    _disp_z(coupledValue("disp_z")), _disp_z_old(coupledValueOld("disp_z")),
    _beta(getParam<Real>("beta")),
    _gamma(getParam<Real>("gamma"))
{}

void
NewmarkMaterial::initQpStatefulProperties()
{
  // init the stateful properties (these will become _bla_old in the first call of computeProperties)
  _vel[_qp] = 0.;
  _acc[_qp] = 0.;
}

void
NewmarkMaterial::computeQpProperties()
{
  const Point u       = Point(_disp_x[_qp],    _disp_y[_qp],    _disp_z[_qp]);
  const Point u_old   = Point(_disp_x_old[_qp],_disp_y_old[_qp],_disp_z_old[_qp]);

  _acc[_qp] =  1./_beta*( (u-u_old)/(_dt*_dt) - _vel_old[_qp]/_dt - _acc_old[_qp]*(0.5-_beta) );
  _vel[_qp] = _vel_old[_qp] + _dt*( (1.-_gamma)*_acc_old[_qp] + _gamma*_acc[_qp] );
  
  _jacobian[_qp] = 1./(_beta*_dt*_dt);
}
