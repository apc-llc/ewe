/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef INCOMPRESSIBILITYDETERMINANT_H
#define INCOMPRESSIBILITYDETERMINANT_H

#include "Kernel.h"
#include "RankTwoTensor.h"

//Forward Declarations
class IncompressibilityDeterminant;

/**
 * validParams returns the parameters that this Kernel accepts / needs
 * The actual body of the function MUST be in the .C file.
 */
template<>
InputParameters validParams<IncompressibilityDeterminant>();

class IncompressibilityDeterminant : public Kernel
{
public:

  IncompressibilityDeterminant(const std::string & name,
                   InputParameters parameters);

protected:
  virtual Real computeQpResidual();
/*  virtual Real computeQpJacobian();
*/
  /**
   * Member reference to the computed values
   * for the deformation gradient tensor
   */
  MaterialProperty<RankTwoTensor> & _F;

};
#endif //INCOMPRESSIBILITYDETERMINANT_H
