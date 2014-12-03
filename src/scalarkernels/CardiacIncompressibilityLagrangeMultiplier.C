/*
 * This kernel was inspired by moose/test/src/scalarkernels/PostprocessorCED.C
 */

#include "CardiacIncompressibilityLagrangeMultiplier.h"

template<>
InputParameters validParams<CardiacIncompressibilityLagrangeMultiplier>()
{
  InputParameters params = validParams<ScalarKernel>();
  params.addRequiredParam<PostprocessorName>("volume_ratio_postprocessor", "Name of the postprocessor that supplies the volume ratio Int{det[F]dV}/V0.");

  return params;
}

CardiacIncompressibilityLagrangeMultiplier::CardiacIncompressibilityLagrangeMultiplier(const std::string & name, InputParameters parameters) :
    ScalarKernel(name, parameters),
    _volume_ratio(getPostprocessorValue("volume_ratio_postprocessor"))
{}

CardiacIncompressibilityLagrangeMultiplier::~CardiacIncompressibilityLagrangeMultiplier()
{}

void
CardiacIncompressibilityLagrangeMultiplier::reinit()
{}

void
CardiacIncompressibilityLagrangeMultiplier::computeResidual()
{
  DenseVector<Number> & re = _assembly.residualBlock(_var.number());
  for (_i = 0; _i < re.size(); _i++)
    re(_i) += computeQpResidual();
}

Real
CardiacIncompressibilityLagrangeMultiplier::computeQpResidual()
{
  return _volume_ratio - 1.;
}

void
CardiacIncompressibilityLagrangeMultiplier::computeJacobian()
{
  DenseMatrix<Number> & ke = _assembly.jacobianBlock(_var.number(), _var.number());
  for (_i = 0; _i < ke.m(); _i++)
    ke(_i, _i) += computeQpJacobian();
}

Real
CardiacIncompressibilityLagrangeMultiplier::computeQpJacobian()
{
  /// @todo TODO: I really would assume that the Jacobian is 0. (or some very high order small number).
  /// For sure, 1. is not correct but I inserted it to force the iterative solver to actually do something.
  return 1.;
}

void
CardiacIncompressibilityLagrangeMultiplier::computeOffDiagJacobian(unsigned int /*jvar*/)
{}

Real
CardiacIncompressibilityLagrangeMultiplier::computeQpOffDiagJacobian(unsigned int /*jvar*/)
{
  return 0.;
}
