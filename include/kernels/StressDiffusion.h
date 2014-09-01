/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef STRESSDIFFUSION_H
#define STRESSDIFFUSION_H

#include "Kernel.h"
#include "RankTwoTensor.h"

//Forward Declarations
class StressDiffusion;

/**
 * validParams returns the parameters that this Kernel accepts / needs
 * The actual body of the function MUST be in the .C file.
 */
template<>
InputParameters validParams<StressDiffusion>();

class StressDiffusion : public Kernel
{
public:

  StressDiffusion(const std::string & name,
                   InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  /**
   * Member reference to the computed values
   * for the 2nd Piola-Kirchoff stress tensor 
   */
  MaterialProperty<RankTwoTensor> & _T;

  /**
   * Member reference for storing the spatial
   * component, this kernel works on 
   */
  MooseEnum _c;
};
#endif //STRESSDIFFUSION_H
