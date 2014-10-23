#ifndef IIONMODEL
#define IIONMODEL

#include <vector>

class Iionmodel {

public:

  Iionmodel() {};
  
  // destructor declared virtual to ensure proper polymorphic delete
  virtual ~Iionmodel() {};
  
  virtual double ionforcing(double) = 0;
  
  virtual void update_gates_dt(double) = 0;
  
};


#endif // IIONMODEL