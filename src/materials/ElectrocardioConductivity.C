/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "ElectrocardioConductivity.h"

template<>
InputParameters validParams<ElectrocardioConductivity>()
{
  InputParameters params = validParams<Material>();
  params.addParam<Real>("conductivity_coefficient", 1.0, "Factor in front of diffusion term");
  return params;
}

ElectrocardioConductivity::ElectrocardioConductivity(const std::string & name,
                                 InputParameters parameters) :
    Material(name, parameters),
    _conductivity_coefficient(getParam<Real>("conductivity_coefficient")),
    _conductivity(declareProperty<Real>("conductivity"))
{
  std::cout << "Instantiating ElectrocardioConductivity ... conductivity_factor = " << _conductivity_coefficient << std::endl;
}

void
ElectrocardioConductivity::computeQpProperties()
{
  _conductivity[_qp] = _conductivity_coefficient;
}
