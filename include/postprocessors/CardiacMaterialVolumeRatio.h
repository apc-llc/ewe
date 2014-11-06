#ifndef CardiacMaterialVolumeRatio_H
#define CardiacMaterialVolumeRatio_H

#include "ElementIntegralPostprocessor.h"

class CardiacMaterialVolumeRatioPostprocessor;

template<>
InputParameters validParams<CardiacMaterialVolumeRatioPostprocessor>();

/**
 * Postprocessor for computing the global volume ratio, i.e. \f$\frac{1}{V}\int J\mathrm{d}V\f$,
 * where the local volume rations \f$J\f$ are fetched from a CardiacNash2000Material instance.
 */
class CardiacMaterialVolumeRatioPostprocessor : public ElementIntegralPostprocessor
{
public:
  CardiacMaterialVolumeRatioPostprocessor(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpIntegral();

  MaterialProperty<Real> & _J;
};

#endif
