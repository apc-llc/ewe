#ifndef CardiacIncompressibilityLagrangeMultiplier_H
#define CardiacIncompressibilityLagrangeMultiplier_H

#include "ScalarKernel.h"

class CardiacIncompressibilityLagrangeMultiplier;

template<>
InputParameters validParams<CardiacIncompressibilityLagrangeMultiplier>();

/**
 *
 */
class CardiacIncompressibilityLagrangeMultiplier : public ScalarKernel
{
public:
  CardiacIncompressibilityLagrangeMultiplier(const std::string & name, InputParameters parameters);
  virtual ~CardiacIncompressibilityLagrangeMultiplier();

  virtual void reinit();
  virtual void computeResidual();
  virtual void computeJacobian();
  virtual void computeOffDiagJacobian(unsigned int jvar);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  unsigned int _i;

  const PostprocessorValue & _volume_ratio;
};


#endif /* CardiacIncompressibilityLagrangeMultiplier_H */
