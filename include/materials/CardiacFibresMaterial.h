/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef CardiacFibresMaterial_H
#define CardiacFibresMaterial_H

#include "Material.h"

//Forward Declarations
class CardiacFibresMaterial;

template<>
InputParameters validParams<CardiacFibresMaterial>();

/**
 * 
 */
class CardiacFibresMaterial : public Material
{
public:
  CardiacFibresMaterial(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeQpProperties();

private:
  inline RealVectorValue VectorNormalize(const RealVectorValue &v) { return v / v.size(); }
  inline RealVectorValue VectorProduct(const RealVectorValue &a, const RealVectorValue &b) { return RealVectorValue(a(1)*b(2) - a(2)*b(1), a(2)*b(0) - a(0)*b(2), a(0)*b(1) - a(1)*b(0)); }

  MaterialProperty<RealVectorValue> & _Ef, & _En, & _Es; // unit vectors for fibre direction, sheet normal and sheet axis (all three are perpendicular to each other, see [Holzapfel 2009, Figure 1] for details
  MaterialProperty<RealTensorValue> & _Rf; // rotation matrix from cartesian to fibre coordinate system. contains columnwise Ef, Es, En

  const RealTensorValue _id;
  const bool _has_fixed_R;
  const RealTensorValue & _fixed_R;

};

#endif //CardiacFibresMaterial_H
