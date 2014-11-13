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

typedef struct{
  unsigned char mcode;
  unsigned char ccode;
  float*param; // TODO: I do not see where this is initialized. Maybe it will be best to keep all necessary parameter sets somewhere in global storage and use only pointers here instead of allocating an array for every element
}Membrane_cell_info;

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
  virtual void computeQpConductivities();
  virtual void computeQpCellInfo();
private:
  MaterialProperty<Membrane_cell_info> & _cell_info; ///< membrane cell info contains mcode (membrane model code), ccode (membrane-model-specific cell type code), param (pointer to/array[ion_mi[mc].Nparam] of membrane-model specific parameters for individual nodes)
  MaterialProperty<RealTensorValue> & _Rf;    ///< rotation matrix from cartesian to fibre coordinate system. contains columnwise \f$\hat{e}_f\f$, \f$\hat{e}_n\f$, \f$\hat{e}_s\f$
  MaterialProperty<RealVectorValue> & _sigma; ///< conductivity rotated from fibre coordinate system as in prm_substance[] into outer cartesian coordinate system
  
  int _SubstanceID; ///< ID for accessing the selected substance in prm_substance[]
  Real _sigmat, _sigmal; ///< transverse and longitudinal conductivities for the selected substance and domain
};

#endif //CardiacPropertiesMaterial_H
