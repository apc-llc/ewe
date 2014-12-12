#include "ElectrocardioForcing.h"
#include "Function.h"

template<>
InputParameters validParams<ElectrocardioForcing>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<Real>("ion_coeff", 1.0, "Auxiliary factor before ion current forcing term: Default 1.0, set to zero to deactivate ion current forcing");
  params.addRequiredParam<FunctionName>("forcing_function", "A function that describes the (time and position dependent) stimulus. For multi stimulus protocols you will want to use PicewiseParsedFunction.");
  return params;
}


ElectrocardioForcing::ElectrocardioForcing(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _ion_coeff(getParam<Real>("ion_coeff")),
    _Iion(getMaterialProperty<Real>("Iion")),
    _forcing_function(getFunction("forcing_function"))
{
  if (_ion_coeff == 0.0)
    mooseWarning("DEACTIVATED ion current forcing since ion_coeff==0.0...");
}

ElectrocardioForcing::~ElectrocardioForcing()
{
}

Real
ElectrocardioForcing::computeQpResidual()
{
  // Externally applied current
  Real _Iion_app = _forcing_function.value(_t, _q_point[_qp]);

  return _ion_coeff*(_Iion[_qp] + _Iion_app)*_test[_i][_qp];
}

Real
ElectrocardioForcing::computeQpJacobian()
{
  return 0.0; 
}
