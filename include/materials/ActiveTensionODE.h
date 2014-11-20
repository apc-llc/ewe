#ifndef ActiveTensionODE_H
#define ActiveTensionODE_H

#include "Material.h"
#include "Iionmodel.h"
#include "IionmodelFactory.h"

class ActiveTensionODE;

template<>
InputParameters validParams<ActiveTensionODE>();

/**
 * Implementation of the relation of active tension \f$T_a\f$
 * to the potential \f$V\f$.
 * The approach follows [Nash2004, eqn (22c)]:
 * \f$\dot{T}_a = \epsilon(\tilde{V})(k_{T_a}\tilde{V}-T_a)\f$
 * where
 * \f$\tilde{V}\$ is a normalized (to \f$[0;1]\f$) potential and
 * \f$\epsilon(\tilde{V}) &= \begin{cases}
 *        \epsilon_{\mathrm{development}}&\mathrm{for}\;\tilde{V}<0.05\\
 *        \epsilon_{\mathrm{recovery}}&\mathrm{for}\;\tilde{V}\geq0.05
 *                           \end{cases}\f$
 * accounts for different time scales for development of and
 * recovery from active tension.
 *
 * Integration is performed by a simple forward Euler method.
 *
 * This material is essentially the coupling facility from
 * Electrocardio to Cardiac mechanics effects.
 *
 * Note that [Nash2004] states that essentially all parameters
 * in this approach are empiric. Thus, tune them to your needs
 * via the input file.
 */
class ActiveTensionODE : public Material
{
public:
  ActiveTensionODE(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeQpProperties();
  virtual void initQpStatefulProperties();

private:

  MaterialProperty<Real> & _Ta;     ///< active tension
  MaterialProperty<Real> & _Ta_old; ///< its old value
  VariableValue & _Vmem;            ///< membrane potential from ElectrocardioDiffusion kernel

  const Real _epsilon_recovery, _epsilon_development, _Vrest, _Vmax, _kTa;

};

#endif //ActiveTensionODE_H
