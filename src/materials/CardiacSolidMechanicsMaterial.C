/*************************************
 * Template for this file came from moose/modules/solid_mechanics/src/materials/total_strain/SolidMechanicsMaterial.C
 *************************************/

#include "CardiacSolidMechanicsMaterial.h"
#include "Problem.h"

#include "VolumetricModel.h"

template<>
InputParameters validParams<CardiacSolidMechanicsMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredCoupledVar("displacements", "The x, y, and z displacement");
  params.addCoupledVar("c","variable that zeros out the stiffness");

  return params;
}

CardiacSolidMechanicsMaterial::CardiacSolidMechanicsMaterial(const std::string & name, InputParameters parameters)
  :Material(name, parameters),
   _has_c(isCoupled("c")),
   _c( _has_c ? coupledValue("c") : _zero),
   _stress(declareProperty<SymmTensor>("stress")),
   _elasticity_tensor(declareProperty<SymmElasticityTensor>("elasticity_tensor")),
   _Jacobian_mult(declareProperty<SymmElasticityTensor>("Jacobian_mult")),
   _elastic_strain(declareProperty<SymmTensor>("elastic_strain"))
{
  // see http://mooseframework.org/wiki/Faq/#coupling-to-an-arbitrary-number-of-variables-back-to-top for details on this magic
  _grad_disp.resize(coupledComponents("displacements"));

  mooseAssert(_grad_disp.size() == 3, "CardiacSolidMechanicsMaterial: displacements must have exactly 3 components");

  for (unsigned int i=0; i<_grad_disp.size(); ++i) {
    _grad_disp[i] = &coupledGradient("displacements", i);
  }
}

