#ifndef CardiacVolumeRatioBase_H
#define CardiacVolumeRatioBase_H

#include "ElementIntegralPostprocessor.h"

class CardiacVolumeRatioBase;

template<>
InputParameters validParams<CardiacVolumeRatioBase>();

/**
 * Postprocessor for computing the global volume ratio, i.e. \f$\frac{1}{V}\int J\mathrm{d}V\f$,
 * where the local volume rations \f$J\f$ are computed by derived class instances.
 * This base class only provides infrastructure for averaging over the total volume.
 * The formulation for averaging over elements (i.e. volume sum etc.) came from 
 * #ElementAverageValue .
 */
class CardiacVolumeRatioBase : public ElementIntegralPostprocessor
{
public:
  CardiacVolumeRatioBase(const std::string & name, InputParameters parameters);

  virtual void initialize();
  virtual void execute();
  virtual Real getValue();
  virtual void threadJoin(const UserObject & y);

protected:
  virtual Real computeQpIntegral() = 0;

  Real _volume;
};

#endif
