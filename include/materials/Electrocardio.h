/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef ELECTROCARDIO_H
#define ELECTROCARDIO_H

#include "Material.h"
#include "Iionmodel.hpp"

//Forward Declarations
class Electrocardio;

template<>
InputParameters validParams<Electrocardio>();

/**
 * 
 */
class Electrocardio : public Material
{
public:
  Electrocardio(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeProperties();
  virtual void computeQpProperties();
  virtual void initQpStatefulProperties();

private:

  MaterialProperty<Real> & _Iion; // ionic currents
  MaterialProperty<std::vector<Real> > & _gates, & _gates_old; // gating variables
  VariableValue & _vmem; // membrane potential
  
  Iionmodel * _ionmodel;
    
};

#endif //ELECTROCARDIO_H
