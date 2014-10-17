/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef ELECTROCARDIOCONDUCTIVITY_H
#define ELECTROCARDIOCONDUCTIVITY_H

#include "Material.h"

//Forward Declarations
class ElectrocardioConductivity;

template<>
InputParameters validParams<ElectrocardioConductivity>();

/**
 * 
 */
class ElectrocardioConductivity : public Material
{
public:
  ElectrocardioConductivity(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeQpProperties();

private:
  
  double _conductivity_coefficient;
  
  // TODO: Have to make conductivity a tensor... - should be possible to fetch from CardiacPropertiesMaterial
  MaterialProperty<Real> & _conductivity;
};

#endif //ELECTROCARDIOCONDUCTIVITY_H
