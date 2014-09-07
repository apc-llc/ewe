/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef ACCVELSTORINGMATERIAL_H
#define ACCVELSTORINGMATERIAL_H

#include "Material.h"

//Forward Declarations
class AccVelStoringMaterial;

template<>
InputParameters validParams<AccVelStoringMaterial>();

/**
 * Example material class that defines a few properties.
 */
class AccVelStoringMaterial : public Material
{
public:
  AccVelStoringMaterial(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeQpProperties();

private:
  // properties to be provided
  MaterialProperty<Point> & _force;
  MaterialProperty<Point> & _df_du;
  // FIXME; we actually do not need to export these as kernel-accessible MaterialProperties, only force is relevant
  //        this might save the _old arrays completely (using scalar temporary variables in computeQpProperties() 
  MaterialProperty<Point> & _accel_new;
  MaterialProperty<Point> & _accel_old;
  MaterialProperty<Point> & _vel_new;
  MaterialProperty<Point> & _vel_old;
  // internal variables for faster access
  const VariableValue & _disp_x;
  const VariableValue & _disp_y;
  const VariableValue & _disp_z;
  const VariableValue & _disp_x_old;
  const VariableValue & _disp_y_old;
  const VariableValue & _disp_z_old;
  const Real _beta;
  const Real _gamma;
  const Real _density;
};

#endif //ACCVELSTORINGMATERIAL_H
