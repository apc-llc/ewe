#include "EweApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"

#include "SecondOrderImplicitEulerWithDensity.h"
#include "SecondDerivativeNewmark.h"
#include "FirstDerivativeNewmark.h"
#include "KineticEnergyNewmarkAux.h"
#include "KineticEnergyAux.h"
#include "NewmarkMaterial.h"
#include "CardiacStressDivergence.h"
#include "CardiacLinearIsotropicMaterial.h"

#include "Electrocardio.h"
#include "ElectrocardioForcing.h"
#include "ElectrocardioTimeDerivative.h"
#include "ElectrocardioDiffusion.h"
#include "ElectrocardioConductivity.h"
#include "ElectrocardioIC.h"
#include "CardiacPropertiesMaterial.h"

template<>
InputParameters validParams<EweApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

EweApp::EweApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(processor_id());

  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  EweApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  EweApp::associateSyntax(_syntax, _action_factory);
}

EweApp::~EweApp()
{
}

void
EweApp::registerApps()
{
  registerApp(EweApp);
}

void
EweApp::registerObjects(Factory & factory)
{
  // Register any custom objects you have built on the MOOSE Framework
  registerKernel(SecondOrderImplicitEulerWithDensity);
  registerKernel(SecondDerivativeNewmark);
  registerKernel(FirstDerivativeNewmark);
  registerKernel(CardiacStressDivergence);
  registerKernel(ElectrocardioForcing);

  registerMaterial(NewmarkMaterial);
  registerMaterial(Electrocardio);
  registerMaterial(ElectrocardioConductivity);
  registerMaterial(CardiacPropertiesMaterial);
  registerMaterial(CardiacLinearIsotropicMaterial);

  registerKernel(ElectrocardioTimeDerivative);
  registerKernel(ElectrocardioDiffusion);

  registerInitialCondition(ElectrocardioIC);
  
  registerAux(KineticEnergyNewmarkAux);
  registerAux(KineticEnergyAux);
}

void
EweApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
