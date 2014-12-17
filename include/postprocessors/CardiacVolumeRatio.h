#ifndef CardiacVolumeRatio_H
#define CardiacVolumeRatio_H

#include "CardiacVolumeRatioBase.h"

class CardiacVolumeRatioPostprocessor;

template<>
InputParameters validParams<CardiacVolumeRatioPostprocessor>();

/**
 * Postprocessor for computing the global volume ratio, i.e. \f$\frac{1}{V}\int J\mathrm{d}V\f$,
 * where the local volume ratios \f$J\f$ are computed from local displacement gradients
 * instead of being fetched from a material property as #CardiacMaterialVolumeRatioPostprocessor does.
 */
class CardiacVolumeRatioPostprocessor : public CardiacVolumeRatioBase
{
public:
  CardiacVolumeRatioPostprocessor(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpIntegral();

  std::vector<VariableGradient *> _grad_disp;
};

#endif
