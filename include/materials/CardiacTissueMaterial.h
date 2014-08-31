/****************************************************************/
/*                                                              */
/*  Implementation of Cardiac Tissue elasticity as given in     */
/*  [Whiteley2007]                                              */
/*                                                              */
/****************************************************************/

#ifndef CARDIACTISSUEMATERIAL_H
#define CARDIACTISSUEMATERIAL_H

#include "Material.h"
#include "SymmTensor.h"
#include "libmesh/tensor_value.h"

//Forward Declarations
class CardiacTissueMaterial;

template<>
InputParameters validParams<CardiacTissueMaterial>();

/**
 * Example material class that defines a few properties.
 */
class CardiacTissueMaterial : public Material
{
public:
  CardiacTissueMaterial(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeQpProperties();

private:
  SymmTensor _k, _a, _b;

  /**
   * Member reference to the (static) values
   * for the orientation of the fibre coordinate system
   */
  MaterialProperty<RealTensorValue> & _P;

  /**
   * Member reference to the computed values
   * for the deformation gradient tensor
   */
  MaterialProperty<RealTensorValue> & _F;
  
  /**
   * Member reference to the computed values
   * for the 2nd Piola-Kirchoff stress tensor 
   */
  MaterialProperty<RealTensorValue> & _T;
};

#endif //CARDIACTISSUEMATERIAL_H
