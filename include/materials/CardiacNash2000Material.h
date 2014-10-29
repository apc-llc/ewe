/****************************************************************/
/*                                                              */
/*  Implementation of Cardiac Tissue elasticity as given in     */
/*  [Nash2000]                                              */
/*                                                              */
/****************************************************************/

#ifndef CardiacNash2000Material_H
#define CardiacNash2000Material_H

#include "Material.h"
#include "SymmTensor.h"
#include "SymmGenericElasticityTensor.h"

//Forward Declarations
class CardiacNash2000Material;
class SymmElasticityTensor;

template<>
InputParameters validParams<CardiacNash2000Material>();

class CardiacNash2000Material : public Material
{
public:
  CardiacNash2000Material(const std::string & name,
                          InputParameters parameters);

protected:
  virtual void computeQpProperties();
  
  const RealTensorValue STtoRTV(const SymmTensor & A) const;
  const SymmGenericElasticityTensor STtoSGET(const SymmTensor & A) const;
  const SymmTensor symmProd(const RealTensorValue & outer, const SymmTensor & inner) const;
  const SymmTensor symmProd(const RealTensorValue & outer) const;
  const SymmTensor symmInv(const SymmTensor & C, const Real det) const;

  VariableGradient & _grad_dispx;
  VariableGradient & _grad_dispy;
  VariableGradient & _grad_dispz;

  SymmTensor _k, _a, _b;

  MaterialProperty<RealTensorValue> & _stress;
  MaterialProperty<SymmGenericElasticityTensor> & _stress_derivative;

  MaterialProperty<Real> & _J;
  MaterialProperty<RealTensorValue> & _Rf;
  bool _has_Ta;
  VariableValue & _Ta;
  bool _has_Ta_function;
  Function * const _Ta_function;
  bool _has_p;
  VariableValue & _p;

  // identity matrix for convenience
  const SymmTensor _id;
};

#endif //CardiacNash2000Material_H
