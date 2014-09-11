/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef NEWMARKMATERIAL_H
#define NEWMARKMATERIAL_H

#include "Material.h"

//Forward Declarations
class NewmarkMaterial;

template<>
InputParameters validParams<NewmarkMaterial>();

/**
 * 
 */
class NewmarkMaterial : public Material
{
public:
  NewmarkMaterial(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeQpProperties();
  virtual void initQpStatefulProperties();

private:

  MaterialProperty<Real> & _vel, & _vel_old;
  MaterialProperty<Real> & _acc, & _acc_old;
  MaterialProperty<Real> & _jacobian;

  const VariableValue & _disp, & _disp_old;
  const Real _beta;
  const Real _gamma;
};

#endif //NEWMARKMATERIAL_H
