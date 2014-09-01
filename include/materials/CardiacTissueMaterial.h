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
#include "RankTwoTensor.h"

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
  /**
   * Member reference to the (static) values
   * for the orientation of the fibre coordinate system
   */
  MaterialProperty<RankTwoTensor> & _P;

  /**
   * Member reference to the computed values
   * for the deformation gradient tensor
   */
  MaterialProperty<RankTwoTensor> & _F;
  
  /**
   * Member reference to the computed values
   * for the 2nd Piola-Kirchoff stress tensor 
   */
  MaterialProperty<RankTwoTensor> & _T;


  SymmTensor _k, _a, _b;
};

#endif //CARDIACTISSUEMATERIAL_H
