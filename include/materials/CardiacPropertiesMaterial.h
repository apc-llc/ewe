/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef CardiacPropertiesMaterial_H
#define CardiacPropertiesMaterial_H

#include "Material.h"

//Forward Declarations
class CardiacPropertiesMaterial;

template<>
InputParameters validParams<CardiacPropertiesMaterial>();

/**
 * 
 */
class CardiacPropertiesMaterial : public Material
{
public:
  CardiacPropertiesMaterial(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeProperties();
  virtual void computeSubstanceProperties();
  virtual void computeQpProperties();
  virtual void computeQpFibreCoordinates();
  virtual void computeQpConductivities();
  virtual void computeQpCellInfo();
private:
  inline RealVectorValue VectorNormalize(const RealVectorValue &v) { return v / v.size(); }
  inline RealVectorValue VectorProduct(const RealVectorValue &a, const RealVectorValue &b) { return RealVectorValue(a(1)*b(2) - a(2)*b(1), a(2)*b(0) - a(0)*b(2), a(0)*b(1) - a(1)*b(0)); }


  MaterialProperty<RealVectorValue> & _Ef, & _En, & _Es; // unit vectors for fibre direction, sheet normal and sheet axis (all three are perpendicular to each other, see [Holzapfel 2009, Figure 1] for details
  MaterialProperty<RealTensorValue> & _Rf; // rotation matrix from cartesian to fibre coordinate system. contains columnwise Ef, Es, En
  MaterialProperty<RealVectorValue> & _sigma; // conductivity rotated from fibre coordinate system as in prm_substance[] into outer cartesian coordinate system
  
  int _SubstanceID; // ID for accessing the selected substance in prm_substance[]
  const std::string & _PropagParams; // command line parameters given funneled to propag
  Real _sigmat, _sigmal; // transverse and longitudinal conductivities for the selected substance and domain
};

#endif //CardiacPropertiesMaterial_H
