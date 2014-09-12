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

  std::string _comp_name;

  MaterialProperty<Real> & _acc, & _acc_old;
  MaterialProperty<Real> & _vel, & _vel_old;
  MaterialProperty<Real> & _gamma, & _beta;
  const VariableValue & _disp, & _disp_old;
  Real __gamma, __beta;
};

#endif //NEWMARKMATERIAL_H
