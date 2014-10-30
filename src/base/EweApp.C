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
#include "CardiacKirchhoffStressDivergence.h"
#include "CardiacLinearIsotropicMaterial.h"
#include "CardiacLinearOrthotropicMaterial.h"
#include "CardiacNash2000Material.h"
#include "PressureLagrangeMultiplier.h"
#include "DisplacementAux.h"
#include "AnisotropicGradientShift.h"

#include "Electrocardio.h"
#include "ElectrocardioForcing.h"
#include "ElectrocardioTimeDerivative.h"
#include "ElectrocardioDiffusion.h"
#include "ElectrocardioConductivity.h"
#include "ElectrocardioIC.h"
#include "CardiacPropertiesMaterial.h"
#include "CardiacFibresMaterial.h"

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
  registerKernel(CardiacKirchhoffStressDivergence);
  registerKernel(ElectrocardioForcing);
  registerKernel(PressureLagrangeMultiplier);
  registerKernel(AnisotropicGradientShift);

  registerMaterial(NewmarkMaterial);
  registerMaterial(Electrocardio);
  registerMaterial(ElectrocardioConductivity);
  registerMaterial(CardiacPropertiesMaterial);
  registerMaterial(CardiacLinearIsotropicMaterial);
  registerMaterial(CardiacLinearOrthotropicMaterial);
  registerMaterial(CardiacNash2000Material);
  registerMaterial(CardiacFibresMaterial);

  registerKernel(ElectrocardioTimeDerivative);
  registerKernel(ElectrocardioDiffusion);

  registerInitialCondition(ElectrocardioIC);
  
  registerAux(KineticEnergyNewmarkAux);
  registerAux(KineticEnergyAux);
  registerAux(DisplacementAux);
}

void
EweApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
