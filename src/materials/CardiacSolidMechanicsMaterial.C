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
  params.addParam<std::string>("appended_property_name", "", "Name appended to material properties to make them unique");
  params.addRequiredCoupledVar("disp_x", "The x displacement");
  params.addRequiredCoupledVar("disp_y", "The y displacement");
  params.addCoupledVar("disp_z", "The z displacement");
  params.addCoupledVar("temp", "The temperature if you want thermal expansion.");
  params.addCoupledVar("c","variable that zeros out the stiffness");

  return params;
}

CardiacSolidMechanicsMaterial::CardiacSolidMechanicsMaterial(const std::string & name, InputParameters parameters)
  :Material(name, parameters),
   _appended_property_name( getParam<std::string>("appended_property_name") ),
   _grad_disp_x(coupledGradient("disp_x")),
   _grad_disp_y(coupledGradient("disp_y")),
   _grad_disp_z(_mesh.dimension() == 3 ? coupledGradient("disp_z") : _grad_zero),
   _has_c(isCoupled("c")),
   _c( _has_c ? coupledValue("c") : _zero),
   _stress(createProperty<SymmTensor>("stress")),
   _elasticity_tensor(createProperty<SymmElasticityTensor>("elasticity_tensor")),
   _Jacobian_mult(createProperty<SymmElasticityTensor>("Jacobian_mult")),
   _elastic_strain(createProperty<SymmTensor>("elastic_strain"))
{}

