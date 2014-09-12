/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "NewmarkMaterial.h"

template<>
InputParameters validParams<NewmarkMaterial>()
{
  InputParameters params = validParams<Material>();

  params.addParam<Real>("alpha", 0.5, "Newmark integration alpha parameter (default: 0.5 --> yields 2nd order method)");

  params.addCoupledVar("disp", "Displacement component");
  params.addRequiredParam<std::string>("str_append", "String to append to output material property names (connection to SecondDerivativeNewmark kernel)");

  return params;
}

NewmarkMaterial::NewmarkMaterial(const std::string & name,
                                 InputParameters parameters) :
    Material(name, parameters),
    _comp_name((getVar("disp", 0))->name()),  // we use the name that was supplied as disp parameter to ensure unique naming and finally correctly correspondence in SecondDerivativeNewmark.C
    _acc(declareProperty<Real>(       "newmark_acceleration-" + _comp_name)),
    _acc_old(declarePropertyOld<Real>("newmark_acceleration-" + _comp_name)),
    _jac(declareProperty<Real>(       "newmark_jacobian-"     + _comp_name)),
    _disp(coupledValue("disp")),
    _disp_old(coupledValueOld("disp")),
    _alpha(getParam<Real>("alpha"))
{}

void
NewmarkMaterial::initQpStatefulProperties()
{
  // init the stateful properties (these will become _bla_old before/in the first call of computeProperties)
  _acc[_qp] = -1.;
}

void
NewmarkMaterial::timeStepSetup()
{
  // this function is being called before every timestep. However, apparently, acessing _vel, _vel_old, etc is not possible here...
  // (they contain invalid values and values written there do not end up in ComputeQpProperties...)
}

void
NewmarkMaterial::computeQpProperties()
{
  if (_t_step == -1) {
    // special treatment in first timestep ??
    
  } else
  {
    _acc[_qp] =  _disp[_qp]/(_alpha*_dt*_dt)  +  ( (1.-_alpha)*_dt*_acc_old[_qp] - (_dt+_dt_old)*_disp_old[_qp] ) / (_alpha*_dt*_dt*_dt_old);
    _jac[_qp] = 1./(_alpha*_dt*_dt);
  }
}
