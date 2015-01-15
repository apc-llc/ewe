#ifndef LinearIsotropicLargeStrainMaterial_H
#define LinearIsotropicLargeStrainMaterial_H

#include "SolidMechanicsMaterial.h"

//Forward Declarations
class LinearIsotropicLargeStrainMaterial;
class SymmElasticityTensor;

template<>
InputParameters validParams<LinearIsotropicLargeStrainMaterial>();

/**
 * LinearIsotropic material for use in simple applications that don't need material properties.
 * This is an extension of the original LinearIsotropicMaterial from
 * MOOSE's solid_mechanics module that includes quadratic
 * displacement terms.
 */
class LinearIsotropicLargeStrainMaterial : public SolidMechanicsMaterial
{
public:
  LinearIsotropicLargeStrainMaterial(const std::string & name,
                          InputParameters parameters);

  virtual ~LinearIsotropicLargeStrainMaterial();

protected:
  virtual void computeProperties();

  virtual void computeStress(const SymmTensor & strain,
                             SymmTensor & stress);

  /**
   * Will always be passed to full symmetric strain tensor.
   * What should come out is a modified strain tensor.
   */
  virtual void computeStrain(const SymmTensor & total_strain, SymmTensor & elastic_strain);

  virtual Real computeAlpha();

  Real _youngs_modulus;
  Real _poissons_ratio;

  bool _large_strain;

  Real _t_ref;
  Real _alpha;

  SymmElasticityTensor * _local_elasticity_tensor;

  Real _pi;
  Real _tol;

};

#endif //LinearIsotropicLargeStrainMaterial_H
