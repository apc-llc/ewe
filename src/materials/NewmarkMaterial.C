/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "NewmarkMaterial.h"

template<>
InputParameters validParams<NewmarkMaterial>()
{
  InputParameters params = validParams<Material>();

  params.addParam<Real>("gamma", 0.5, "Newmark integration gamma parameter (default: 0.5)");
  params.addParam<Real>("beta", 0.25, "Newmark integration beta parameter (default: 0.25)");
  params.addCoupledVar("disp", "Displacement component");
  //params.set<bool>("use_displaced_mesh") = true;

  return params;
}

NewmarkMaterial::NewmarkMaterial(const std::string & name,
                                 InputParameters parameters) :
    Material(name, parameters),
    _comp_name((getVar("disp", 0))->name()),  // we use the name that was supplied as disp parameter to ensure unique naming and finally correctly correspondence in SecondDerivativeNewmark.C
    _acc(declareProperty<Real>(       "newmark_acceleration-" + _comp_name)),
    _acc_old(declarePropertyOld<Real>("newmark_acceleration-" + _comp_name)),
    _vel(declareProperty<Real>(       "newmark_velocity-"     + _comp_name)),
    _vel_old(declarePropertyOld<Real>("newmark_velocity-"     + _comp_name)),
    _gamma(declareProperty<Real>(     "newmark_gamma-"        + _comp_name)),
    _beta( declareProperty<Real>(     "newmark_beta-"         + _comp_name)),
    // coupled variables
    _disp(coupledValue("disp")),
    _disp_old(coupledValueOld("disp")),
    // parameters
    __gamma(getParam<Real>("gamma")),
    __beta(getParam<Real>("beta"))
{}

void
NewmarkMaterial::initQpStatefulProperties()
{
  // init the stateful properties (these will become _bla_old before/in the first call of computeProperties)
  _acc[_qp] = 0;
  _vel[_qp] = 0;
}

void
NewmarkMaterial::computeQpProperties()
{
  // this looks strange but this is exactly as ConstantPropertyMaterial.C does it
  _gamma[_qp] = __gamma;
  _beta[_qp]  = __beta;

  // FIXME, actually this kernel should only be called at the end of every timestep
  _acc[_qp] = ( _disp[_qp]/(_dt*_dt) - _vel_old[_qp]/_dt - (0.5-_beta[_qp])*_acc_old[_qp] ) / _beta[_qp]; 
  _vel[_qp] = _vel_old[_qp] + _dt*( (1-_gamma[_qp])*_acc_old[_qp] + _gamma[_qp]*_acc[_qp] );
}
