#ifndef CardiacKirchhoffIncompressibilityPenalty_H
#define CardiacKirchhoffIncompressibilityPenalty_H

#include "Kernel.h"
#include "SymmGenericElasticityTensor.h"

//Forward Declarations
class ColumnMajorMatrix;
class CardiacKirchhoffIncompressibilityPenalty;
class SymmElasticityTensor;
class SymmTensor;

template<>
InputParameters validParams<CardiacKirchhoffIncompressibilityPenalty>();

class CardiacKirchhoffIncompressibilityPenalty : public Kernel
{
public:

  CardiacKirchhoffIncompressibilityPenalty(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  Real JacobianElement(unsigned int comp);

  MaterialProperty<RealTensorValue> & _F;
  MaterialProperty<Real> & _J;

private:
  const unsigned int _component;
  const Real _gamma;

  const unsigned int _xdisp_var;
  const unsigned int _ydisp_var;
  const unsigned int _zdisp_var;
};
#endif //CardiacKirchhoffIncompressibilityPenalty_H
