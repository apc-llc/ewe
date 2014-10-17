/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#ifndef ELECTROCARDIO_H
#define ELECTROCARDIO_H

#include "Material.h"
#include "ion.h"
#include "membrane.h"

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
  MaterialProperty<Real> & _dtime, & _dtime_old; // depolarization times
  MaterialProperty<std::vector<Real> > & _yyy, & _yyy_old; // gating variables
  MaterialProperty<Membrane_cell_info> & _cell_info; // cell type info from CardiacPropertiesMaterial
  VariableValue & _vmem; // membrane potential
  int _ndep, _nnd, _all_dep;
  const std::string & _PropagParams; // command line parameters given funneled to propag
    
};

#endif //ELECTROCARDIO_H
