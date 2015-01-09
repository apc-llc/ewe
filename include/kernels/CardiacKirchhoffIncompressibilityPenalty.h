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

/**
 * Kernel for adding a penalty term to a variable's residual for enforcing incompressibility.
 *
 * The kernel implements \f$\gamma(\psi_i,J-1)\f$ with 
 * \f$J=\mathrm{det}\mathbf{F}=\mathrm{det}\frac{\partial x_M}{\partial X_N}\f$,
 * the displaced coordinates \f$x_{M=1\ldots3}\f$ and 
 * the coordinates in the reference frame \f$X_{N=1\ldots3}\f$.
 *
 * The value for \f$J\f$ is fetched from a #CardiacMechanicsMaterial instance
 * via its `det_displacement_gradient` MaterialProperty.
 * Furthermore, the `displacement_gradient` MaterialProperty is used in the
 * kernel's Jacobian computation.
 */
class CardiacKirchhoffIncompressibilityPenalty : public Kernel
{
public:

  CardiacKirchhoffIncompressibilityPenalty(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  Real JacobianElement(unsigned int comp);

  MaterialProperty<RealTensorValue> & _F; ///< `displacement_gradient` of the corresponding CardiacNash2000Material
  MaterialProperty<Real> & _J;            ///< `det_displacement_gradient` of the corresponding CardiacNash2000Material

private:
  const unsigned int _component;          ///< cartesian component this kernel works on (to extract correct components of _F in Jacobian computation)
  const Real _gamma;                      ///< Kernel prefactor for tuning penalty magnitude relative to other residuals

  /// indices of the three coupled displacement variables
  unsigned int _disp_var[3];
};
#endif //CardiacKirchhoffIncompressibilityPenalty_H
