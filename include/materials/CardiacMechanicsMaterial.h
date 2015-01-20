#ifndef CardiacMechanicsMaterial_H
#define CardiacMechanicsMaterial_H

#include "Material.h"
#include "SymmTensor.h"
#include "SymmGenericElasticityTensor.h"

class CardiacMechanicsMaterial;

template<>
InputParameters validParams<CardiacMechanicsMaterial>();

/**
 * Generic Material for Cadiac Mechanics.
 * It cares for inclusion of fibre directions,
 * active tension and pressure into the 2nd Piola-Kirchhoff
 * stress tensor, see \ref Whiteley2007 and \ref Holzapfel2009
 * for details.
 *
 * The tensor itself has to be computed in a derived
 * class from the supplied rotated Lagrange-Green strain
 *
 */
class CardiacMechanicsMaterial : public Material
{
public:
  CardiacMechanicsMaterial(const std::string & name,
                          InputParameters parameters);

protected:
  virtual void computeQpProperties();

  /**
   * This function has to be implemented by
   * subclasses. It must compute _stress[_qp],
   * stress_derivative[_qp] and _W[_qp]
   */
  virtual void computeQpStressProperties(const SymmTensor &C, const SymmTensor &E) = 0;

  std::vector<VariableGradient *> _grad_disp;

  MaterialProperty<SymmTensor> & _stress; ///< 2nd Piola-Kirchhoff stress tensor T_MN
  MaterialProperty<SymmGenericElasticityTensor> & _stress_derivative; ///< derivative of T_MN wrt E_PQ

  MaterialProperty<RealTensorValue> & _F; ///< displacement gradient
  MaterialProperty<Real> & _J; ///< det F, i.e. volume change
  MaterialProperty<SymmTensor> & _Cinv; ///< inverse of the Cauchy-Green deformation tensor in the outer coordinate system (only computed if the hydrostatic pressure p is given)
  MaterialProperty<RealTensorValue> & _Finv; ///< inverse of the displacement gradient
  MaterialProperty<Real> & _W; ///< elastic energy
  MaterialProperty<RealTensorValue> & _Rf; ///< fibre orientation rotation matrix
  MaterialProperty<RealVectorValue> & _Ef, & _Es, & _En; ///< fibre basis vectors
  bool _has_Ta;
  VariableValue & _Ta;
  bool _has_Ta_function;
  Function * const _Ta_function;
  bool _has_p;
  VariableValue & _p;

  const SymmTensor _id; ///< identity matrix just for convenience

};

#endif //CardiacMechanicsMaterial_H
