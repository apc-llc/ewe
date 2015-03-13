#include "ElectrocardioIC.h"
#include <math.h>

template<>
InputParameters validParams<ElectrocardioIC>()
{
  InputParameters params = validParams<InitialCondition>();
  return params;
}

ElectrocardioIC::ElectrocardioIC(const std::string & name,
                     InputParameters parameters) :
    InitialCondition(name, parameters)
{
}

ElectrocardioIC::~ElectrocardioIC()
{
}

Real
ElectrocardioIC::value(const Point & p)
{
  /// The value -90.272 is the resting potential of the bernus model for all except one case...
  /// \todo TODO: make this more general: different ion models have different resting potentials
 // return -90.272;
  return -92.189; // for some reason, my Bernus model has a slightly different resting potential.. probably a slighly wrong parameter?
}
