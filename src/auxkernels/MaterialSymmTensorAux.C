#include "MaterialSymmTensorAux.h"


template<>
InputParameters validParams<MaterialAuxBase<SymmTensor> >()
{
  return validParams<MaterialAuxBase<RealTensorValue> >();
}

template<>
InputParameters validParams<MaterialSymmTensorAux>()
{
  InputParameters params = validParams<MaterialAuxBase<SymmTensor> >();
  params.addParam<unsigned int>("row", 0, "The row component to consider for this kernel");
  params.addParam<unsigned int>("column", 0, "The column component to consider for this kernel");
  return params;
}

MaterialSymmTensorAux::MaterialSymmTensorAux(const std::string & name, InputParameters parameters) :
    MaterialAuxBase<SymmTensor>(name, parameters),
    _row(getParam<unsigned int>("row")),
    _col(getParam<unsigned int>("column"))
{
  if (_row > LIBMESH_DIM)
    mooseError("The row component " << _row << " does not exist for " << LIBMESH_DIM << " dimensional problems");
  if (_col > LIBMESH_DIM)
    mooseError("The column component " << _col << " does not exist for " << LIBMESH_DIM << " dimensional problems");
}

MaterialSymmTensorAux::~MaterialSymmTensorAux()
{
}

Real
MaterialSymmTensorAux::computeValue()
{
  return _factor * _prop[_qp](_row, _col) + _offset;
}
