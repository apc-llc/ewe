#ifndef CardiacFibresMaterial_H
#define CardiacFibresMaterial_H

#include "Material.h"

class CardiacFibresMaterial;

template<>
InputParameters validParams<CardiacFibresMaterial>();

/**
 * Material for providing an interface to fibre direction
*
 * Material properties are:
 *  - the local coordinate system's basis vectors \f$\hat{e}_f\f$, \f$\hat{e}_n\f$, \f$\hat{e}_s\f$
 *    (see notation in \ref Holzapfel2009 "[Holzapfel, 2009, Figure 1]")
 *  - an appropriate rotation matrix \f$\mathbf{R}=(\hat{e}_f, \hat{e}_n, \hat{e}_s)\f$, i.e.
 *    containing the unit vectors column-wise
 * For debugging purposes, the rotation matrix (and respective coordinate
 * system) can also be given externally in the input file.
 *
 * The local fibre coordinate system is constructed in an analogous
 * fashion to the description in \ref Potse2006
 * with the difference that we do not average over neighbouring elements
 * for getting a smoothed thickness parameter e.
 * Instead, we directly use the result from a CardiacThicknessParameterAux
 * kernel.
 * For further details consult the publication and see into the
 * CardiacThicknessParameterAux and VolumeNearestNodeAux kernels.
 */
class CardiacFibresMaterial : public Material
{
public:
  CardiacFibresMaterial(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeQpProperties();

private:
  /// Computes \f$\frac{\vec{v}}{|\vec{v}|}\f$.
  inline RealVectorValue VectorNormalize(const RealVectorValue &v) { return v / v.size(); }

  MaterialProperty<RealVectorValue> & _Ef, & _Es, & _En; ///< unit vectors for fibre direction, sheet axis and sheet normal (all three are perpendicular to each other, see \ref Holzapfel2009 "[Holzapfel, 2009, Figure 1]" for details
  MaterialProperty<RealTensorValue> & _Rf;               ///< rotation matrix from cartesian to fibre coordinate system. contains row-wise \f$\hat{e}_f\f$, \f$\hat{e}_s\f$, \f$\hat{e}_n\f$

  const RealTensorValue _id;
  const bool _has_fixed_R;
  const RealTensorValue & _fixed_R;
  const bool _has_e;
  const VariableValue & _e;
  const VariableGradient & _grad_e;

};

#endif //CardiacFibresMaterial_H
