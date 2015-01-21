#ifndef CardiacKirchhoffIncompressibilityLagrangeMultiplier_H
#define CardiacKirchhoffIncompressibilityLagrangeMultiplier_H

#include "Kernel.h"

//Forward Declarations
class ColumnMajorMatrix;
class CardiacKirchhoffIncompressibilityLagrangeMultiplier;
class SymmElasticityTensor;
class SymmTensor;

template<>
InputParameters validParams<CardiacKirchhoffIncompressibilityLagrangeMultiplier>();

/**
 * Kernel for enforcing incompressibility by means of a Lagrange multiplier \f$p\f$ per element.
 *
 * The kernel implements \f$(\psi_i,J-1)\f$ with 
 * \f$J=\mathrm{det}\mathbf{F}=\mathrm{det}\frac{\partial x_M}{\partial X_N}\f$,
 * the displaced coordinates \f$x_{M=1\ldots3}\f$ and 
 * the coordinates in the reference frame \f$X_{N=1\ldots3}\f$.
 *
 * The value for \f$J\f$ is fetched from a #CardiacMechanicsMaterial instance
 * via its `det_displacement_gradient` MaterialProperty.
 * Furthermore, the `displacement_gradient` MaterialProperty is used in the
 * kernel's Jacobian computation.
 */
class CardiacKirchhoffIncompressibilityLagrangeMultiplier : public Kernel
{
public:

  CardiacKirchhoffIncompressibilityLagrangeMultiplier(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  MaterialProperty<RealTensorValue> & _F; ///< `displacement_gradient` of the corresponding CardiacNash2000Material
  MaterialProperty<RealTensorValue> & _invF; ///< `inv_displacement_gradient` of the corresponding CardiacNash2000Material
  MaterialProperty<Real> & _J;            ///< `det_displacement_gradient` of the corresponding CardiacNash2000Material

private:
  /// indices of the three coupled displacement variables
  unsigned int _disp_var[3];
};
#endif //CardiacKirchhoffIncompressibilityLagrangeMultiplier_H
