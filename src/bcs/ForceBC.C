/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "ForceBC.h"

template<>
InputParameters validParams<ForceBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  MooseEnum component("X=0, Y=1, Z=2");
  params.addRequiredParam<MooseEnum>("component", component, "Component, this BC works on (selects row in 2ndPiolaKirchoff stress tensor): X, Y, or Z.");

  return params;
}

ForceBC::ForceBC(const std::string & name, InputParameters parameters) :
    IntegratedBC(name, parameters),
    _T(getMaterialProperty<RankTwoTensor>("PiolaKirchoff2nd")),
    _c(getParam<MooseEnum>("component"))
{}

Real
ForceBC::computeQpResidual()
{
  return -_test[_i][_qp]*(_T[_qp].row(_c)*_grad_u[_qp]);
}
