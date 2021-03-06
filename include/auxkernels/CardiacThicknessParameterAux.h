#ifndef CardiacThicknessParameterAux_H
#define CardiacThicknessParameterAux_H

#include "AuxKernel.h"

class CardiacThicknessParameterAux;

template<>
InputParameters validParams<CardiacThicknessParameterAux>();

/**
 * AuxKernel for computing a thickness parameter for a cardiac model.
 *
 * This is essentially the same as given in Section II.A of
 * \ref Potse2006 with the following differences:
 * We compute \f$e=\frac{d_\mathrm{epi}}{d_\mathrm{endo}+d_\mathrm{epi}}\f$,
 * i.e. \f$e=0\f$ on the epicardium, \f$|e|=1\f$ on the endocardium.
 * Furthermore, we optionally switch signs of \f$e\f$: It is positive for
 * nodes that are closer to the left ventricle's epicardium than
 * to the right ventricle's and negative otherwise.
 *
 * Note, that the required distances will ususally be computed
 * using VolumeNearestNodeDistanceAux kernels.
 */
class CardiacThicknessParameterAux : public AuxKernel
{
public:

  CardiacThicknessParameterAux(const std::string & name, InputParameters parameters);

  virtual ~CardiacThicknessParameterAux() {}

protected:
  virtual Real computeValue();

  const VariableValue & _d_rv, & _d_lv, & _d_o;
  const Real _rvfac;

};

#endif //CardiacThicknessParameterAux_H
