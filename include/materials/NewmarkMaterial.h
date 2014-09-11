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

  MaterialProperty<Point> & _delta_a;
  MaterialProperty<Point> & _vel, & _vel_old;
  MaterialProperty<Point> & _acc, & _acc_old;
  MaterialProperty<Point> & _jacobian;

  const VariableValue & _disp_x, & _disp_x_old;
  const VariableValue & _disp_y, & _disp_y_old;
  const VariableValue & _disp_z, & _disp_z_old;
  const Real _beta;
  const Real _gamma;
};

#endif //NEWMARKMATERIAL_H
