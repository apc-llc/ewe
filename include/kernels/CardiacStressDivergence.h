/*************************************
 * Template for this file came from moose/modules/solid_mechanics/include/kernels/StressDivergence.h
 *************************************/

#ifndef CardiacStressDivergence_H
#define CardiacStressDivergence_H

#include "Kernel.h"

//Forward Declarations
class ColumnMajorMatrix;
class CardiacStressDivergence;
class SymmElasticityTensor;
class SymmTensor;

template<>
InputParameters validParams<CardiacStressDivergence>();

class CardiacStressDivergence : public Kernel
{
public:

  CardiacStressDivergence(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  MaterialProperty<SymmTensor> & _stress;
  MaterialProperty<SymmElasticityTensor> & _Jacobian_mult;

private:
  const unsigned int _component;

  /// indices of the three coupled displacement variables
  unsigned int _disp_var[3];
};
#endif //CardiacStressDivergence_H
