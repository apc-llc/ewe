#ifndef CardiacVolumeRatio_H
#define CardiacVolumeRatio_H

#include "ElementIntegralPostprocessor.h"

class CardiacVolumeRatioPostprocessor;

template<>
InputParameters validParams<CardiacVolumeRatioPostprocessor>();

/**
 * Postprocessor for computing the global volume ratio, i.e. \f$\frac{1}{V}\int J\mathrm{d}V\f$,
 * where the local volume ratios \f$J\f$ are computed from local displacement gradients.
 */
class CardiacVolumeRatioPostprocessor : public ElementIntegralPostprocessor
{
public:
  CardiacVolumeRatioPostprocessor(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpIntegral();

  VariableGradient & _grad_dispx, & _grad_dispy, & _grad_dispz;
};

#endif
