/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef ELECTROCARDIO_H
#define ELECTROCARDIO_H

#include "Material.h"
#include "Iionmodel.h"
#include "IionmodelFactory.h"

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
  
  std::vector<double> gates_qp, gates_dt_qp;
  
  //! @todo: this has to be a MaterialProperty as well with one object per point
  Iionmodel * _ionmodel;
    
};

#endif //ELECTROCARDIO_H
