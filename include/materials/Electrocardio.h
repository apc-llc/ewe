/**
* The Electrocardio material allows to couple a ion channel
* model to a diffusion PDE to model electrocardiac activity.
*
*
*
*/

#ifndef ELECTROCARDIO_H
#define ELECTROCARDIO_H

#include "Material.h"
#include "Iionmodel.h"
#include "IionmodelFactory.h"

// #include "CudaIionmodel.h"
// #include "CudaIionmodelFactory.h"

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

  virtual ~Electrocardio() { }

protected:
  virtual void computeProperties();
  virtual void computeQpProperties();
  virtual void initQpStatefulProperties();

private:

  MaterialProperty<Real> & _Iion; // ionic currents
  MaterialProperty<std::vector<Real> > & _gates, & _gates_old; // gating variables
  VariableValue & _vmem; // membrane potential
    
  //CudaIionmodel * _ionmodel;
  Iionmodel * _ionmodel;
    
};

#endif //ELECTROCARDIO_H
