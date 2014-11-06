/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef ELECTROCARDIOFORCING_H
#define ELECTROCARDIOFORCING_H

#include "Kernel.h"

class ElectrocardioForcing;

template<>
InputParameters validParams<ElectrocardioForcing>();

/**
 * @todo docu
 *
 * Mono-domain equation:
 *
 * \\( \\partial_t V - \\frac{1}{C_m \\cdot \\chi } \\nabla \\cdot \\left( G_{\\rm mono} \\nabla V \\right) + \\left( I_{\\rm ion} + I_{\\rm app} \\right) = 0 \\)
 *
 * @note The ion currents \\( I_{\\rm ion} \\) and \\( I_{\\rm app} \\) are supposed to be in \\( \\textrm{mV}~\\textrm{ms}^{-1} \\) which is equal to \\( \\textrm{mV}~\\mu\\textrm{S}~\\textrm{nF}^{-1} \\) or \\( \\textrm{pA}~\\textrm{pF}^{-1} \\).
 * @note To convert these values to \\( \\textrm{A}~\\textrm{m}^{-2} \\), multiply with \\( C_{m} \\) in \\( \\mu\\textrm{F}~\\textrm{cm}^{-2} \\).
 *
 * Units of involved quantities:
 *
 * - Length: \\( \\left[ x \\right] = \\textrm{cm}  \\)
 * - Time: \\( \\left[ t \\right] = \\textrm{ms}  \\)
 * - Potential: \\( \\left[ V \\right]  = \\textrm{mV}  \\)
 * - Capacitance: \\( \\left[ C_{m} \\right] = \\mu \\textrm{F}~\\textrm{cm}^{-2} \\)
 * - Conductivity: \\( \\left[ G \\right] = \\textrm{S}~\\textrm{cm}^{-1}  \\)
 * - Surface-to-volume ratio: \\( \\left[ \\chi \\right] = \\textrm{cm}^{-1} \\)
 * - Ion current per unit area: \\( \\left[ I_{\\rm ion} \\right] = p\\textrm{A}~\\textrm{cm}^{-2} \\)
 *
 * Both Siemens (S) and Faraday (F) read in SI units:
 *
 * - Siemens \\( \\textrm{S} = \\frac{ \\textrm{A} }{ \\textrm{V} } = 10^{-3} \\frac{ \\textrm{A} }{ \\textrm{mV} } \\)
 * - Faraday \\( \\textrm{F} = \\frac{ \\textrm{A}~\\textrm{s} }{ \\textrm{V} } = \\frac{ \\textrm{A}~\\textrm{ms} }{\\textrm{mV}} \\)
 * - Current density \\( \\textrm{pA}~\\textrm{pF}^{-1} = \\textrm{mV}~\\textrm{ms}^{-1} \\)
 *
 * This results in the following physical units of the terms in the mono domain equation:
 *
 * - \\( \\left[ \\partial_t V \\right] = \\textrm{mV}~\\textrm{ms}^{-1} \\)
 * - \\( \\left[ C_m^{-1} G_{\\rm mono} \\right] = \\mu\\textrm{F}^{-1}~\\textrm{cm}^{2}~\\textrm{S}~\\textrm{cm}^{-1} = 10^{3}~\\textrm{cm}~\\textrm{ms}^{-1} \\)
 * - \\( \\left[ \\nabla \\cdot \\nabla V \\right] = \\textrm{mV}~\\textrm{cm}^{-2} \\)
 * - \\( \\left[ \\chi^{-1} \\left( C_{m}^{-1}  G_{\\rm mono} \\right) \\nabla \\cdot \\left( \\nabla V \\right) \\right] =  10^{3}~\\textrm{mV}~\\textrm{ms}^{-1} \\)
 * - \\( \\left[ I_{\\rm ion} \\right] = p\\textrm{A}~p\\textrm{F}^{-1} = \\textrm{mV}~\\textrm{ms}^{-1} \\)
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
  
};
  
#endif /* ELECTROCARDIOFORCING_H */
