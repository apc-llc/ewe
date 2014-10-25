/****************************************************************/
/*                                                              */
/*  Implementation of Cardiac Tissue elasticity as given in     */
/*  [Whiteley2007]                                              */
/*                                                              */
/****************************************************************/

#ifndef CardiacWhiteley2007Material_H
#define CardiacWhiteley2007Material_H

#include "Material.h"
#include "SymmTensor.h"
#include "SymmElasticityTensor.h"

//Forward Declarations
class CardiacWhiteley2007Material;
class SymmElasticityTensor;

template<>
InputParameters validParams<CardiacWhiteley2007Material>();

class CardiacWhiteley2007Material : public Material
{
public:
  CardiacWhiteley2007Material(const std::string & name,
                          InputParameters parameters);

protected:
  virtual void computeQpProperties();
  
  const RealTensorValue STtoRTV(const SymmTensor & A) const;
  const SymmElasticityTensor STtoSET(const SymmTensor & A) const;
  const SymmTensor symmProd(const RealTensorValue & outer, const SymmTensor & inner) const;
  const SymmTensor symmProd(const RealTensorValue & outer) const;
  const SymmTensor symmInv(const SymmTensor & C, const Real det) const;

  VariableGradient & _grad_disp_x;
  VariableGradient & _grad_disp_y;
  VariableGradient & _grad_disp_z;

  SymmTensor _k, _a, _b;

  MaterialProperty<RealTensorValue> & _stress;
  MaterialProperty<SymmElasticityTensor> & _stress_derivative; // TODO: get rid of SymmElasticityTensor - it offerst too many features we do not need and makes things complicated

  MaterialProperty<Real> & _J;
  MaterialProperty<RealTensorValue> & _Rf;
  bool _has_Ta;
  VariableValue & _Ta;
  bool _has_p;
  VariableValue & _p;

  // identity matrix for convenience
  const SymmTensor _id;
};

#endif //CardiacWhiteley2007Material_H
