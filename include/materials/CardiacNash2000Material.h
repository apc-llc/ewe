#ifndef CardiacNash2000Material_H
#define CardiacNash2000Material_H

#include "Material.h"
#include "SymmTensor.h"
#include "SymmGenericElasticityTensor.h"

class CardiacNash2000Material;

template<>
InputParameters validParams<CardiacNash2000Material>();

/**
 * Implementation of the 'pole-free' hyperelastic material law
 * for cardiac tissue as given in [Nash 2000].
 *
 * Parameters are read via MaterialParameters from the
 * input file. A linear material behavior (for debugging)
 * can be realized by setting the approriate \f$k_{ij}\f$ to
 * negative values, see CardiacNash2000Material::computeQpProperties()
 * for details.
 */
class CardiacNash2000Material : public Material
{
public:
  CardiacNash2000Material(const std::string & name,
                          InputParameters parameters);

protected:
  virtual void computeQpProperties();

  VariableGradient & _grad_dispx;
  VariableGradient & _grad_dispy;
  VariableGradient & _grad_dispz;

  SymmTensor _k, _a, _b;

  MaterialProperty<RealTensorValue> & _stress;
  MaterialProperty<SymmGenericElasticityTensor> & _stress_derivative;

  MaterialProperty<RealTensorValue> & _F;
  MaterialProperty<Real> & _J;
  MaterialProperty<Real> & _W;
  MaterialProperty<RealTensorValue> & _Rf;
  bool _has_Ta;
  VariableValue & _Ta;
  bool _has_Ta_function;
  Function * const _Ta_function;
  bool _has_p;
  VariableValue & _p;

  const SymmTensor _id; ///< identity matrix just for convenience

};

#endif //CardiacNash2000Material_H
