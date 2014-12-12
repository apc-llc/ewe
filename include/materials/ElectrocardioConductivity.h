#ifndef ELECTROCARDIOCONDUCTIVITY_H
#define ELECTROCARDIOCONDUCTIVITY_H

#include "Material.h"
#include "libmesh/tensor_value.h"

class ElectrocardioConductivity;

template<>
InputParameters validParams<ElectrocardioConductivity>();

/**
 * Material for supplying a conductivity tensor.
 * \f$G=\sum_{i=f,s,n}\sigma_i(\vec{e}_i\otimes\vec{e}_i)\f$
 * Conductivities in fibre, sheet and sheet-normal directions are read from the input file.
 * For sensible values, see e.g. [Potse, 2006, Table I].
 *
 * @todo Actually, we should distinguish different domains and substances as it is done
 * in propag (where in contrast to here only fibre-longitudinal and -transversal
 * conductivities are considered):
 * @code
 *   set_default_substance(0,"void",0.0,0.0,0.0,0.0);
 *   set_default_substance(1,"ventricle",3.0,1.2,3.0,0.3);
 *   set_default_substance(2,"atrium",3.0,1.2,3.0,0.3);
 *   set_default_substance(3,"inactive",2.0,2.0,0.0,0.0);
 *   set_default_substance(4,"ischemic",3.0,1.2,3.0,0.3);
 *   set_default_substance(5,"blood",6.0,6.0,0.0,0.0);
 *   set_default_substance(6,"His bundle",3.0,1.2,3.0,0.3);
 *   set_default_substance(7,"AV node",3.0,1.2,3.0,0.3);
 *
 *   ...
 *
 *   if(cdom==DOM_INTRA){
 *   sigmac= prm_substance[s].sigma_ic;
 *   sigmat= prm_substance[s].sigma_it;
 *   sigmal= prm_substance[s].sigma_il;
 *   }else if(cdom==DOM_EXTRA){
 *   sigmac= prm_substance[s].sigma_ec;
 *   sigmat= prm_substance[s].sigma_et;
 *   sigmal= prm_substance[s].sigma_el;
 *   }else if(cdom==DOM_MONO){
 *   sigmac= gmono(prm_substance[s].sigma_ic,prm_substance[s].sigma_ec);
 *   sigmat= gmono(prm_substance[s].sigma_it,prm_substance[s].sigma_et);
 *   sigmal= gmono(prm_substance[s].sigma_il,prm_substance[s].sigma_el);
 *   }else if(cdom==DOM_BULK){
 *   sigmac= gbulk(prm_substance[s].sigma_ic,prm_substance[s].sigma_ec);
 *   sigmat= gbulk(prm_substance[s].sigma_it,prm_substance[s].sigma_et);
 *   sigmal= gbulk(prm_substance[s].sigma_il,prm_substance[s].sigma_el);
 *   }else{
 *   Error(22,"This cannot happen: unknown domain=%d\n",cdom);
 *   }
 *
 *   ...
 *
 *   #define set_default_substance(n,s,Sel,Set,Sil,Sit)  \
 *   if(strcmp(prm_substance[n].name,"none") ==0) { \
 *   prm_substance[n].name= s; \
 *   prm_substance[n].sigma_el= Sel; \
 *   prm_substance[n].sigma_et= Set; \
 *   prm_substance[n].sigma_ec= -1.0; \
 *   prm_substance[n].sigma_il= Sil; \
 *   prm_substance[n].sigma_it= Sit; \
 *   prm_substance[n].sigma_ic= -1.0; \
 *   } \
 *
 *   ...
 *
 *   #define gbulk(i,e) ((i) +(e) ) 
 *   #define gmono(i,e) ((i) +(e) ==0?0.0:(i) *(e) /((i) +(e) ) )  \
 *
 * @endcode
 */
class ElectrocardioConductivity : public Material
{
public:
  ElectrocardioConductivity(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeQpProperties();

private:

  MaterialProperty<RealVectorValue> & _Ef, & _En, & _Es; ///< unit vectors for fibre direction, sheet normal and sheet axis (all three are perpendicular to each other, see [Holzapfel 2009, Figure 1] for details
  
  std::vector<Real> _conductivities;
  
  MaterialProperty<RealTensorValue> & _conductivity;
};

#endif //ELECTROCARDIOCONDUCTIVITY_H
