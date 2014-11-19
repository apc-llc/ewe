/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "CardiacPropertiesMaterial.h"

template<>
InputParameters validParams<CardiacPropertiesMaterial>()
{
  InputParameters params = validParams<Material>();
  return params;
}

CardiacPropertiesMaterial::CardiacPropertiesMaterial(const std::string & name,
                                 InputParameters parameters) :
  Material(name, parameters),
  _cell_info(declareProperty<Membrane_cell_info>("cell_info")),
  _Rf(getMaterialProperty<RealTensorValue>("R_fibre")),
  _sigma(declareProperty<RealVectorValue>("Sigma")),
  _SubstanceID(-1)
{
  // TODO: Moose creates three material objects: volume, neighbor and boundary for every thread
  // to only initialize propag once (see lines 1580ff in FEProblem.C)
  // we call the init routines only for the volume part of the very first thread.
  // NOTE: This assumes that volume is ALWAYS created first...
  // TODO: I would really not assume that any propag-routines are thread-safe.
  if (_tid == 0 && !_bnd && !_neighbor) {
    
    // @todo TODO: Adopt this to bernus c++ model
  }
}

void CardiacPropertiesMaterial::computeQpConductivities()
{
  _sigma[_qp] = _Rf[_qp] * RealVectorValue(_sigmal, _sigmat, _sigmat);
}


void CardiacPropertiesMaterial::computeQpCellInfo()
{
  // every node corresponds to a cell (ionic properties apply to nodes; materials [e.g. conductivity] apply to elements)
  // there are a few properties of the membrane models that can be specified on a per-node basis; this can happen here

  //! @todo This is the place where spatially-dependent models and celltypes can be introduced

}


void CardiacPropertiesMaterial::computeSubstanceProperties()
{
  //! @todo Initialize correct values here
  _sigmat = 0.0;
  _sigmal = 0.0;
}


void CardiacPropertiesMaterial::computeProperties()
{
  // finding out the correct substance index and conductivities only has to be done once here, not individually for every quadrature point
  computeSubstanceProperties();
  Material::computeProperties();
}

void CardiacPropertiesMaterial::computeQpProperties()
{
  computeQpConductivities();
  computeQpCellInfo();
}
