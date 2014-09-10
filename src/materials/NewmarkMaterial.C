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
    _vel(declareProperty<Point>("newmark_velocity")),
    _acc(declareProperty<Point>("newmark_acceleration")),
    _beta(declareProperty<Real>("newmark_beta")),
    _gamma(declareProperty<Real>("newmark_gamma")),
    _disp_x(coupledValue("disp_x")),
    _disp_y(coupledValue("disp_y")),
    _disp_z(coupledValue("disp_z")),
    __beta(getParam<Real>("beta")),
    __gamma(getParam<Real>("gamma"))
{}

void
NewmarkMaterial::computeQpProperties()
{
  // This way of defining a spatially constant material parameter seems to be insane, but this is exactly what GenericConstantMaterial.C does...
  _beta[_qp]  = __beta;
  _gamma[_qp] = __gamma;

  Point u       = Point(_disp_x[_qp],_disp_y[_qp],_disp_z[_qp]);
  Point aold    = _acc[_qp];
  _acc[_qp]     = u / (_beta[_qp]*_dt) + _delta_a[_qp];
  _vel[_qp]     = _vel[_qp] + _dt*( (1.-_gamma[_qp])*aold + _gamma[_qp]*_acc[_qp] );
  _delta_a[_qp] = -( u/(_dt*_dt) + _vel[_qp]/_dt + (0.5-_beta[_qp])*_acc[_qp] ) / _beta[_qp];
}
