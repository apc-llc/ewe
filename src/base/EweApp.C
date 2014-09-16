#include "EweApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"

#include "SecondOrderImplicitEulerWithDensity.h"
#include "SecondDerivativeNewmark.h"
#include "FirstDerivativeNewmark.h"
#include "KineticEnergyNewmarkAux.h"
#include "NewmarkMaterial.h"

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
  registerAux(KineticEnergyNewmarkAux);
  registerMaterial(NewmarkMaterial);
}

void
EweApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
