#include "ElectrocardioConductivity.h"
#include "TensorHelpers.h"

template<>
InputParameters validParams<ElectrocardioConductivity>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<std::vector<Real> >("conductivities", "Conductivities in fibre, sheet and sheet-normal direction");
  return params;
}

ElectrocardioConductivity::ElectrocardioConductivity(const std::string & name,
                                 InputParameters parameters) :
  Material(name, parameters),
  _Ef(getMaterialProperty<RealVectorValue>("E_fibre")),
  _En(getMaterialProperty<RealVectorValue>("E_normal")),
  _Es(getMaterialProperty<RealVectorValue>("E_sheet")),
  _conductivities(getParam<std::vector<Real> >("conductivities")),
  _conductivity(declareProperty<RealTensorValue>("conductivity"))
{
  if (_conductivities.size() != 3)
    mooseError("ElectrocardioConductivity: conductivities must contain exactly 3 numbers");
}

void
ElectrocardioConductivity::computeQpProperties()
{
  _conductivity[_qp] =  _conductivities[0]*TensorHelpers::kron(_Ef[_qp],_Ef[_qp])
                       +_conductivities[1]*TensorHelpers::kron(_Es[_qp],_Es[_qp])
                       +_conductivities[2]*TensorHelpers::kron(_En[_qp],_En[_qp]);
}
