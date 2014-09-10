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

private:

  MaterialProperty<Point> & _delta_a;
  MaterialProperty<Point> & _vel;
  MaterialProperty<Point> & _acc;
  MaterialProperty<Real>  & _beta;
  MaterialProperty<Real>  & _gamma;

  const VariableValue & _disp_x;
  const VariableValue & _disp_y;
  const VariableValue & _disp_z;
  const Real __beta;
  const Real __gamma;
};

#endif //NEWMARKMATERIAL_H
