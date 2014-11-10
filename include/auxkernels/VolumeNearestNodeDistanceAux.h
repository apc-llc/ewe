#ifndef VolumeNearestNodeDistanceAux_H
#define VolumeNearestNodeDistanceAux_H

#include "AuxKernel.h"
#include "VolumeNearestNodeLocator.h"



class VolumeNearestNodeDistanceAux;

template<>
InputParameters validParams<VolumeNearestNodeDistanceAux>();

/**
 * AuxKernel for computing for all volume nodes the nodes on
 * a given boundary that are closest to them (as well as their distance).
 *
 * This code works totally identical to the NearestNodeDistanceAux kernel.
 * In fact, most of the code here is taken from NearestNodeDistanceAux.h,
 * NearestNodeDistanceAux.C, and GeometricSearchData.C
 * and only had to be slightly modified to fit our needs.
 */
class VolumeNearestNodeDistanceAux : public AuxKernel
{
public:
  VolumeNearestNodeDistanceAux(const std::string & name, InputParameters parameters);

  virtual ~VolumeNearestNodeDistanceAux();

protected:
  virtual Real computeValue();

  VolumeNearestNodeLocator *_nearest_node;
};

#endif //VolumeNearestNodeDistanceAux_H
