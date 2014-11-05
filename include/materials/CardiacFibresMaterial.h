#ifndef CardiacFibresMaterial_H
#define CardiacFibresMaterial_H

#include "Material.h"

class CardiacFibresMaterial;

template<>
InputParameters validParams<CardiacFibresMaterial>();

/**
 * Material for providing an interface to fibre direction (or any other
 * specific local coordinate system).
 *
 * Material properties are:
 *  - the local coordinate system's basis vectors \f$\hat{e}_f\f$, \f$\hat{e}_n\f$, \f$\hat{e}_s\f$
 *    (see notation in [Holzapfel 2009, Figure 1])
 *  - an appropriate rotation matrix \f$\mathbf{R}=(\hat{e}_f, \hat{e}_n, \hat{e}_s)\f$, i.e.
 *    containing the unit vectors column-wise
 * For debugging purposes, the rotation matrix (and respective coordinate
 * system) can also be given externally in the input file.
 *
 * \todo Currently, the local coordinate system is computed to somehow
 * approximately reflect the fibre distribution given in Figure 1 of
 * [Holzapfel 2009]. This is far from being perfect, though and should
 * be replaced by something more realistic.
 * Furthermore, we require the vector field \f$\hat{e}_f\f$ to be divergence free
 * which is currently not the case.
 * See e.g. Marks work on Comparison of mono- and bidomain equation
 * for some hints on how to implement this.
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
  /// Computes \f$ \vec{a} \times \vec{b}\f$.
  inline RealVectorValue VectorProduct(const RealVectorValue &a, const RealVectorValue &b) { return RealVectorValue(a(1)*b(2) - a(2)*b(1), a(2)*b(0) - a(0)*b(2), a(0)*b(1) - a(1)*b(0)); }

  MaterialProperty<RealVectorValue> & _Ef, & _En, & _Es; ///< unit vectors for fibre direction, sheet normal and sheet axis (all three are perpendicular to each other, see [Holzapfel 2009, Figure 1] for details
  MaterialProperty<RealTensorValue> & _Rf;               ///< rotation matrix from cartesian to fibre coordinate system. contains columnwise \f$\hat{e}_f\f$, \f$\hat{e}_n\f$, \f$\hat{e}_s\f$

  const RealTensorValue _id;
  const bool _has_fixed_R;
  const RealTensorValue & _fixed_R;

};

#endif //CardiacFibresMaterial_H
