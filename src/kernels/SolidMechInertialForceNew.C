/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "SolidMechInertialForceNew.h"
#include "SubProblem.h"

template<>
InputParameters validParams<SolidMechInertialForceNew>()
{
  InputParameters params = validParams<Kernel>();
    params.set<bool>("use_displaced_mesh") = true;
    params.addRequiredCoupledVar("acceleration","acceleration variable");
    params.addRequiredParam<Real>("beta","beta parameter");
  return params;
}

SolidMechInertialForceNew::SolidMechInertialForceNew(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
   _density(getMaterialProperty<Real>("density")),
   _accel(coupledValue("acceleration")),
   _beta(getParam<Real>("beta"))
{}

Real
SolidMechInertialForceNew::computeQpResidual()
{
  return _test[_i][_qp]*_density[_qp]*_accel[_qp];
}

Real
SolidMechInertialForceNew::computeQpJacobian()
{
  return _test[_i][_qp]*_density[_qp]*-1/(_beta*(_dt*_dt));
}

