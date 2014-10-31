#ifndef CardiacKirchhoffIncompressibility_H
#define CardiacKirchhoffIncompressibility_H

#include "Kernel.h"
#include "SymmGenericElasticityTensor.h"

//Forward Declarations
class ColumnMajorMatrix;
class CardiacKirchhoffIncompressibility;
class SymmElasticityTensor;
class SymmTensor;

template<>
InputParameters validParams<CardiacKirchhoffIncompressibility>();

class CardiacKirchhoffIncompressibility : public Kernel
{
public:

  CardiacKirchhoffIncompressibility(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  MaterialProperty<RealTensorValue> & _F;
  MaterialProperty<Real> & _J;

private:
  const unsigned int _xdisp_var;
  const unsigned int _ydisp_var;
  const unsigned int _zdisp_var;
};
#endif //CardiacKirchhoffIncompressibility_H
