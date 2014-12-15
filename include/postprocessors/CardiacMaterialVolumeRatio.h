#ifndef CardiacMaterialVolumeRatio_H
#define CardiacMaterialVolumeRatio_H

#include "CardiacVolumeRatioBase.h"

class CardiacMaterialVolumeRatioPostprocessor;

template<>
InputParameters validParams<CardiacMaterialVolumeRatioPostprocessor>();

/**
 * Postprocessor for computing the global volume ratio, i.e. \f$\frac{1}{V}\int J\mathrm{d}V\f$,
 * where the local volume rations \f$J\f$ are fetched from a CardiacNash2000Material instance
 * (in contrast to #CardiacVolumeRatioPostprocessor that computes it explicitly)
 */
class CardiacMaterialVolumeRatioPostprocessor : public CardiacVolumeRatioBase
{
public:
  CardiacMaterialVolumeRatioPostprocessor(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpIntegral();

  MaterialProperty<Real> & _J;
};

#endif
