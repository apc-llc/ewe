/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef FORCEBC_H
#define FORCEBC_H

#include "IntegratedBC.h"
#include "RankTwoTensor.h"

//Forward Declarations
class ForceBC;

template<>
InputParameters validParams<ForceBC>();

/**
 * Implements a simple constant Neumann BC where grad(u)=alpha * v on the boundary.
 * Uses the term produced from integrating the diffusion operator by parts.
 */
class ForceBC : public IntegratedBC
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  ForceBC(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

private:
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

#endif //FORCEBC_H
