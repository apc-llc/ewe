/****************************************************************/
/*        derived from NearestNodeDistanceAux.h                 */
/****************************************************************/

#ifndef VolumeNearestNodeDistanceAux_H
#define VolumeNearestNodeDistanceAux_H

#include "AuxKernel.h"
#include "NearestNodeLocator.h"


//Forward Declarations
class VolumeNearestNodeDistanceAux;

template<>
InputParameters validParams<VolumeNearestNodeDistanceAux>();

/**
 * Constant auxiliary value
 */
class VolumeNearestNodeDistanceAux : public AuxKernel
{
public:
  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  VolumeNearestNodeDistanceAux(const std::string & name, InputParameters parameters);

  virtual ~VolumeNearestNodeDistanceAux();

protected:
  virtual Real computeValue();

  NearestNodeLocator & _nearest_node;
};

#endif //VolumeNearestNodeDistanceAux_H
