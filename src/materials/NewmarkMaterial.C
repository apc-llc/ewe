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

  params.addCoupledVar("disp", "Displacement component");
  params.addRequiredParam<std::string>("str_append", "String to append to output material property names (connection to SecondDerivativeNewmark kernel)");

  return params;
}

NewmarkMaterial::NewmarkMaterial(const std::string & name,
                                 InputParameters parameters) :
    Material(name, parameters),
    _vel(declareProperty<Real>("newmark_velocity" + getParam<std::string>("str_append"))),
    _vel_old(declarePropertyOld<Real>("newmark_velocity" + getParam<std::string>("str_append"))),
    _acc(declareProperty<Real>("newmark_acceleration" + getParam<std::string>("str_append"))),
    _acc_old(declarePropertyOld<Real>("newmark_acceleration" + getParam<std::string>("str_append"))),
    _jacobian(declareProperty<Real>("newmark_jacobian" + getParam<std::string>("str_append"))),
    _disp(coupledValue("disp")),
    _disp_old(coupledValueOld("disp")),
    _beta(getParam<Real>("beta")),
    _gamma(getParam<Real>("gamma"))
{}

void
NewmarkMaterial::initQpStatefulProperties()
{
  // init the stateful properties (these will become _bla_old in the first call of computeProperties)
  _vel[_qp] = -1.;
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
  if (_t_step == 1) {
    // special treatment in first timestep
    
  } else
  {
    _acc[_qp] =  1./_beta*( (_disp[_qp]-_disp_old[_qp])/(_dt*_dt) 
                               - _vel_old[_qp]/_dt
                               - _acc_old[_qp]*(0.5-_beta) );

    _vel[_qp] = _vel_old[_qp] + _dt*( (1.-_gamma)*_acc_old[_qp] + _gamma*_acc[_qp] );

    _jacobian[_qp] = 1./(_beta*_dt*_dt);
  }
}
