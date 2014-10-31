/****************************************************************/
/*                                                              */
/****************************************************************/

#ifndef CardiacMaterialVolumeRatio_H
#define CardiacMaterialVolumeRatio_H

#include "ElementIntegralPostprocessor.h"

class CardiacMaterialVolumeRatioPostprocessor;

template<>
InputParameters validParams<CardiacMaterialVolumeRatioPostprocessor>();

class CardiacMaterialVolumeRatioPostprocessor : public ElementIntegralPostprocessor
{
public:
  CardiacMaterialVolumeRatioPostprocessor(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpIntegral();

  MaterialProperty<Real> & _J;
};

#endif
