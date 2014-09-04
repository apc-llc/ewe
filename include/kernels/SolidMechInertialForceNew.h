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
  MaterialProperty<Real> & _density;
  const VariableValue & _accel;
  const Real _beta;

};
#endif //SOLIDMECHINERTIALFORCENEW_H
