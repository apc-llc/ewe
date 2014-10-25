/*************************************
 * Template for this file came from moose/test/src/kernels/ScalarLagrangeMultiplier.C
 *************************************/

#include "PressureLagrangeMultiplier.h"

template<>
InputParameters validParams<PressureLagrangeMultiplier>()
{
  InputParameters params = validParams<ScalarKernel>();
  return params;
}

PressureLagrangeMultiplier::PressureLagrangeMultiplier(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _J(getMaterialProperty<Real>("det_displacement_gradient"))
{}

Real
PressureLagrangeMultiplier::computeQpResidual()
{
  return _J[_qp] * _test[_i][_qp]; // TODO: this should be the average value of _J across the whole volume, e.g. via AverageMaterialPropertyPostprocessor and VolumePostprocessor
  //return _u * (_J[_qp] - 1) * _test[_i][_qp];
}

void
PressureLagrangeMultiplier::computeOffDiagJacobianScalar(unsigned int jvar)
{
/* TODO
  DenseMatrix<Number> & ken = _assembly.jacobianBlock(_var.number(), jvar);
  DenseMatrix<Number> & kne = _assembly.jacobianBlock(jvar, _var.number());
  MooseVariableScalar & jv = _sys.getScalarVariable(_tid, jvar);

  for (_i = 0; _i < _test.size(); _i++)
    for (_j = 0; _j < jv.order(); _j++)
      for (_qp = 0; _qp < _qrule->n_points(); _qp++)
      {
        Real value = _JxW[_qp] * _coord[_qp] * computeQpOffDiagJacobian(jvar);
        ken(_i, _j) += value;
        kne(_j, _i) += value;
      }
      */
}

Real
PressureLagrangeMultiplier::computeQpOffDiagJacobian(unsigned int jvar)
{
/* TODO:
  if (jvar == _lambda_var)
    return _test[_i][_qp];
  else
  */
    return 0.;
}

