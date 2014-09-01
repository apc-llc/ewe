#include "EweApp.h"
#include "Moose.h"
#include "AppFactory.h"

#include "StressDiffusion.h"
#include "CardiacTissueMaterial.h"
#include "ForceBC.h"
#include "IncompressibilityDeterminant.h"


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
  EweApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
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
  registerKernel(StressDiffusion);
  registerKernel(IncompressibilityDeterminant);
  registerMaterial(CardiacTissueMaterial);
  registerBoundaryCondition(ForceBC);
}

void
EweApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
