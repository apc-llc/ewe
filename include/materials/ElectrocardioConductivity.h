#ifndef ELECTROCARDIOCONDUCTIVITY_H
#define ELECTROCARDIOCONDUCTIVITY_H

#include "Material.h"
#include "libmesh/tensor_value.h"

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

  MaterialProperty<RealVectorValue> & _Ef, & _En, & _Es; ///< unit vectors for fibre direction, sheet normal and sheet axis (all three are perpendicular to each other, see [Holzapfel 2009, Figure 1] for details
  
  std::vector<Real> _conductivities;
  
  MaterialProperty<RealTensorValue> & _conductivity;
};

#endif //ELECTROCARDIOCONDUCTIVITY_H
