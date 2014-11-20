#include "ActiveTensionODE.h"

template<>
InputParameters validParams<ActiveTensionODE>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredCoupledVar("Vmem","Membrane potential as computed by the mono/bidomain equation.");
  params.addParam<Real>("epsilon_recovery", 0.01, "Tension recovery delay parameter.");
  params.addParam<Real>("epsilon_development", 0.04, "Tension development delay parameter.");
  params.addParam<Real>("kTa", 47.9, "Potential to tension conversion factor (regulates maximum tension obtained).");

  params.addParam<Real>("Vrest", -90.272, "Ion model resting potential (for normalization of Vmem).");
  params.addParam<Real>("Vmax", 0., "Ion model maximum potential (for normalization of Vmem).");

  return params;
}

ActiveTensionODE::ActiveTensionODE(const std::string & name,
                                 InputParameters parameters) :
  Material(name, parameters),
  _Ta(declareProperty<Real>("active_tension")),
  _Ta_old(declarePropertyOld<Real>("active_tension")),
  _Vmem(coupledValue("Vmem")),
  _epsilon_recovery(getParam<Real>("epsilon_recovery")),
  _epsilon_development(getParam<Real>("epsilon_development")),
  _Vrest(getParam<Real>("Vrest")),
  _Vmax(getParam<Real>("Vmax")),
  _kTa(getParam<Real>("kTa"))
{
}

void
ActiveTensionODE::initQpStatefulProperties()
{
  // this will automatically be copied to Ta_old[] in the right moment - we do not have to care
  _Ta[_qp] = 0;
}

void
ActiveTensionODE::computeQpProperties()
{
  // normalized potential
  const Real V( (_Vmem[_qp] - _Vrest)/(_Vmax-_Vrest) );
  // simple forward Euler for active tension
  _Ta[_qp] = _dt * (V < 0.05 ? _epsilon_development : _epsilon_recovery) * (_kTa*V-_Ta_old[_qp]) + _Ta_old[_qp];
}
