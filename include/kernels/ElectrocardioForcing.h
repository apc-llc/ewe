#ifndef ELECTROCARDIOFORCING_H
#define ELECTROCARDIOFORCING_H

#include "Kernel.h"

class ElectrocardioForcing;
class Function;

template<>
InputParameters validParams<ElectrocardioForcing>();

/**
 * @todo docu
 *
 * Mono-domain equation:
 *
 * \f$ \partial_t V - \frac{1}{C_m \cdot \chi } \nabla \cdot \left( G_{\rm mono} \nabla V \right) + \left( I_{\rm ion} + I_{\rm app} \right) = 0 \f$
 *
 * @note The ion currents \f$ I_{\rm ion} \f$ and \f$ I_{\rm app} \f$ are supposed to be in \f$ \textrm{mV}~\textrm{ms}^{-1} \f$ which is equal to \f$ \textrm{mV}~\mu\textrm{S}~\textrm{nF}^{-1} \f$ or \f$ \textrm{pA}~\textrm{pF}^{-1} \f$.
 * @note To convert these values to \f$ \textrm{A}~\textrm{m}^{-2} \f$, multiply with \f$ C_{m} \f$ in \f$ \mu\textrm{F}~\textrm{cm}^{-2} \f$.
 *
 * Units of involved quantities:
 *
 * - Length: \f$ \left[ x \right] = \textrm{cm}  \f$
 * - Time: \f$ \left[ t \right] = \textrm{ms}  \f$
 * - Potential: \f$ \left[ V \right]  = \textrm{mV}  \f$
 * - Capacitance: \f$ \left[ C_{m} \right] = \mu \textrm{F}~\textrm{cm}^{-2} \f$
 * - Conductivity: \f$ \left[ G \right] = \textrm{S}~\textrm{cm}^{-1}  \f$
 * - Surface-to-volume ratio: \f$ \left[ \chi \right] = \textrm{cm}^{-1} \f$
 * - Ion current per unit area: \f$ \left[ I_{\rm ion} \right] = p\textrm{A}~\textrm{cm}^{-2} \f$
 *
 * Siemens (S) and Farad (F) read in SI units:
 *
 * - Siemens \f$ \textrm{S} = \frac{ \textrm{A} }{ \textrm{V} } = 10^{-3} \frac{ \textrm{A} }{ \textrm{mV} } \f$
 * - Farad \f$ \textrm{F} = \frac{ \textrm{A}~\textrm{s} }{ \textrm{V} } = \frac{ \textrm{A}~\textrm{ms} }{\textrm{mV}} \f$
 * - Current density \f$ \textrm{pA}~\textrm{pF}^{-1} = \textrm{mV}~\textrm{ms}^{-1} \f$
 *
 * This results in the following physical units of the terms in the mono domain equation:
 *
 * - \f$ \left[ \partial_t V \right] = \textrm{mV}~\textrm{ms}^{-1} \f$
 * - \f$ \left[ C_m^{-1} G_{\rm mono} \right] = \mu\textrm{F}^{-1}~\textrm{cm}^{2}~\textrm{S}~\textrm{cm}^{-1} = 10^{3}~\textrm{cm}~\textrm{ms}^{-1} \f$
 * - \f$ \left[ \nabla \cdot \nabla V \right] = \textrm{mV}~\textrm{cm}^{-2} \f$
 * - \f$ \left[ \chi^{-1} \left( C_{m}^{-1}  G_{\rm mono} \right) \nabla \cdot \left( \nabla V \right) \right] =  10^{3}~\textrm{mV}~\textrm{ms}^{-1} \f$
 * - \f$ \left[ I_{\rm ion} \right] = p\textrm{A}~p\textrm{F}^{-1} = \textrm{mV}~\textrm{ms}^{-1} \f$
 */
class ElectrocardioForcing : public Kernel
{
public:
  
  //! @todo docu
  ElectrocardioForcing(const std::string & name, InputParameters parameters);
  
  //! @todo docu
  virtual ~ElectrocardioForcing();

protected:
  
  //! @todo docu
  virtual Real computeQpResidual();
  
  //! @todo docu
  virtual Real computeQpJacobian();

private:

  //! @todo docu
  double _ion_coeff;
  
  //! @todo docu
  MaterialProperty<Real> & _Iion;

  //! @todo docu
  Function & _forcing_function;
};
  
#endif /* ELECTROCARDIOFORCING_H */
