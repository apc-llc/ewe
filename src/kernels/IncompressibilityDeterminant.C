/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "IncompressibilityDeterminant.h"

/**
 * This function defines the valid parameters for
 * this Kernel and their default values
 */
template<>
InputParameters validParams<IncompressibilityDeterminant>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}


IncompressibilityDeterminant::IncompressibilityDeterminant(const std::string & name,
                                   InputParameters parameters) :
    Kernel(name,parameters),
    _F(getMaterialProperty<RankTwoTensor>("deformation_gradient"))
{}

Real
IncompressibilityDeterminant::computeQpResidual()
{
  return _test[_i][_qp]*(_F[_qp].det()-1.);
}

/* TODO: JAcobian is missing - have fun with this one :-)
Real
IncompressibilityDeterminant::computeQpJacobian()
{
  return ((_grad_test[_i][_qp])*(_T[_qp].row(_c)*_grad_phi[_j][_qp]))(0); // TODO: dito
}
*/
