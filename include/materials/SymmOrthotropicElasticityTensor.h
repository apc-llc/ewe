#ifndef SYMMORTHOTROPICELASTICITYTENSOR_H
#define SYMMORTHOTROPICELASTICITYTENSOR_H

#include "SymmElasticityTensor.h"

/**
 * Defines an Orthotropic Elasticity Tensor.
 *
 * This is a generalization of SymmIsotropicElasticityTensor
 *
 * The input parameters must be the following:
 *
 * Youngs Moduli   E_i   ( i  = 1,2,3    )
 * Poissons Ratios nu_ij ( ij = 21,31,32 )
 * Shear Moduli    G_ij  ( ij = 21,31,32 )
 *
 * Note that by default this tensor is _constant_... meaning
 * that it never changes after the first time it is computed.
 *
 * If you want to modify this behavior you can pass in
 * false to the constructor.
 */
class SymmOrthotropicElasticityTensor : public SymmElasticityTensor
{
public:
  SymmOrthotropicElasticityTensor(const bool constant = true);

  virtual ~SymmOrthotropicElasticityTensor() {}

  void setYoungsModuli(const Real E_1, const Real E_2, const Real E_3);
  void setPoissonsRatios(const Real nu_12, const Real nu_13, const Real nu_23);
  void setShearModuli(const Real G_12, const Real G_13, const Real G_23);

  virtual Real stiffness( const unsigned i, const unsigned j,
                          const RealGradient & test,
                          const RealGradient & phi );

  virtual void multiply( const SymmTensor & x, SymmTensor & b ) const;

protected:

  Real _E1, _E2, _E3, _nu12, _nu13, _nu23, _G12, _G13, _G23;

  virtual void calculateEntries(unsigned int qp);
};

#endif //SYMMORTHOTROPICELASTICITYTENSOR_H
