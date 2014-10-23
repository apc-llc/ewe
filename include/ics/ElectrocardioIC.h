/****************************************************************/
/*                                                              */
/****************************************************************/

#ifndef ELECTROCARDIOIC_H
#define ELECTROCARDIOIC_H

#include "InitialCondition.h"
#include "membrane.h"

// Forward Declarations
class ElectrocardioIC;

template<>
InputParameters validParams<ElectrocardioIC>();

class ElectrocardioIC : public InitialCondition
{
  
public:

  ElectrocardioIC(const std::string & name,
            InputParameters parameters);

  ~ElectrocardioIC();
  
  /**
   * The value of the variable at a point.
   *
   * This must be overriden by derived classes.
   */
  virtual Real value(const Point & p);

private:
  int _activated_cells;
};

#endif //ELECTROCARDIOIC_H