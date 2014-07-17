#ifndef EWEAPP_H
#define EWEAPP_H

#include "MooseApp.h"

class EweApp;

template<>
InputParameters validParams<EweApp>();

class EweApp : public MooseApp
{
public:
  EweApp(const std::string & name, InputParameters parameters);
  virtual ~EweApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* EWEAPP_H */
