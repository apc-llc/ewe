#ifndef CardiacKirchhoffStressDivergence_H
#define CardiacKirchhoffStressDivergence_H

#include "Kernel.h"
#include "SymmGenericElasticityTensor.h"
#include "TensorHelpers.h"

using namespace TensorHelpers;

//Forward Declarations
class ColumnMajorMatrix;
class CardiacKirchhoffStressDivergence;
class SymmElasticityTensor;
class SymmTensor;

template<>
InputParameters validParams<CardiacKirchhoffStressDivergence>();

/**
 * Stress divergence kernel - tightly coupled to CardiacNash2000Material.
 *
 * This kernel implements \f$(\mathrm{grad}\psi_i,\mathbf{T}\mathrm{grad} u_c)\f$ with 
 * the 2nd Piola-Kirchhoff stress tensor \f$\mathbf{T}\f$ that is
 * fetched from a CardiacNash2000Material instance via its
 * `Kirchhoff_stress` MaterialProperty. Furthermore, the
 * `Kirchhoff_stress_derivative` MaterialProperty is utilized in the
 * kernel's Jacobian computation.
 *
 * The idea for this kernel was inspired by StressDivergence.C
 */
class CardiacKirchhoffStressDivergence : public Kernel
{
public:

  CardiacKirchhoffStressDivergence(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  MaterialProperty<RealTensorValue> & _stress;                          ///< 2nd Piola-Kirchhoff stress tensor \f$T_{MN}\f$
  MaterialProperty<SymmGenericElasticityTensor> & _stress_derivative;   ///< derivative of the 2nd Piola-Kirchhoff stress tensor \f$\frac{\partial T_{MN}}{\partial E_{PQ}}\f$
  MaterialProperty<RealTensorValue> & _Rf;                              ///< rotation tensor for the local (fibre) coordinate system

  /// computes   SUM(M,N) [ t(M,N) v1(M) v2(N) ]
  Real fullContraction(const RealTensorValue & t,
                       const RealVectorValue & v1,
                       const RealVectorValue & v2) const;

private:
  const unsigned int _component;

  const VariableGradient & _grad_disp_x;
  const VariableGradient & _grad_disp_y;
  const VariableGradient & _grad_disp_z;

  const unsigned int _xdisp_var;
  const unsigned int _ydisp_var;
  const unsigned int _zdisp_var;
};
#endif //CardiacKirchhoffStressDivergence_H
