#ifndef CardiacKirchhoffStressDivergence_H
#define CardiacKirchhoffStressDivergence_H

#include "Kernel.h"
#include "SymmGenericElasticityTensor.h"

//Forward Declarations
class ColumnMajorMatrix;
class CardiacKirchhoffStressDivergence;
class SymmElasticityTensor;
class SymmTensor;

template<>
InputParameters validParams<CardiacKirchhoffStressDivergence>();

class CardiacKirchhoffStressDivergence : public Kernel
{
public:

  CardiacKirchhoffStressDivergence(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  MaterialProperty<RealTensorValue> & _stress;
  MaterialProperty<SymmGenericElasticityTensor> & _stress_derivative;

  RealVectorValue prod(const RealTensorValue & t, const RealVectorValue & v) const;
  RealTensorValue doubleContraction(const SymmGenericElasticityTensor & t, const RealVectorValue & v1, const RealVectorValue & v2) const;

private:
  const unsigned int _component;

  const unsigned int _xdisp_var;
  const unsigned int _ydisp_var;
  const unsigned int _zdisp_var;
};
#endif //CardiacKirchhoffStressDivergence_H
