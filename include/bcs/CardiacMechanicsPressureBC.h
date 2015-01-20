#ifndef CardiacMechanicsPressureBC_H
#define CardiacMechanicsPressureBC_H

#include "IntegratedBC.h"

//Forward Declarations
class Function;
class CardiacMechanicsPressureBC;

template<>
InputParameters validParams<CardiacMechanicsPressureBC>();

/**
 * This boundary condition prescribes a specific pressure (i.e. normal surface traction per
 * unit area) as a value, a function or product of both.
 * To this end, it has to be applied to all three displacement components
 * with an appropriate `component` parameter value.
 **/
class CardiacMechanicsPressureBC : public IntegratedBC
{
public:

  CardiacMechanicsPressureBC(const std::string & name, InputParameters parameters);

  virtual ~CardiacMechanicsPressureBC(){}

protected:

  virtual Real computeQpResidual();

  const int _component;

  const Real _value;

  Function * const _function;
  const bool _current_config;

  MaterialProperty<Real> & _J; ///< det F, i.e. volume change
  MaterialProperty<RealTensorValue> & _Finv; ///< inverse of the displacement gradient

};

#endif //CardiacMechanicsPressureBC_H
