/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef SOLIDMECHINERTIALFORCENEW_H
#define SOLIDMECHINERTIALFORCENEW_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class SolidMechInertialForceNew;

template<>
InputParameters validParams<SolidMechInertialForceNew>();

class SolidMechInertialForceNew : public Kernel
{
public:

  SolidMechInertialForceNew(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

private:
  const MaterialProperty<Point> & _force;
  const MaterialProperty<Point> & _df_du;
  const MooseEnum _c;

};
#endif //SOLIDMECHINERTIALFORCENEW_H
