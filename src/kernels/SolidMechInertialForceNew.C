/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "SolidMechInertialForceNew.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<SolidMechInertialForceNew>()
{
  InputParameters params = validParams<Kernel>();
  params.set<bool>("use_displaced_mesh") = true;
  MooseEnum component("X=0, Y=1, Z=2");
  params.addRequiredParam<MooseEnum>("component", component, "Cartesian component, this kernel works on: X, Y, or Z.");
  return params;
}

SolidMechInertialForceNew::SolidMechInertialForceNew(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
   _force(getMaterialProperty<Point>("force")),
   _df_du(getMaterialProperty<Point>("dforce_du")),
   _c(getParam<MooseEnum>("component"))
{}

Real
SolidMechInertialForceNew::computeQpResidual()
{
  return _test[_i][_qp]*_force[_qp](_c);
}

Real
SolidMechInertialForceNew::computeQpJacobian()
{
  return _test[_i][_qp]*_df_du[_qp](_c);
}

