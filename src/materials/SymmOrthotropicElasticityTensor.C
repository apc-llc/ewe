/*************************************
 * Template for this file came from moose/modules/solid_mechanics/src/materials/SymmIsotropicElasticityTensor.C
 *************************************/

#include "SymmOrthotropicElasticityTensor.h"

SymmOrthotropicElasticityTensor::SymmOrthotropicElasticityTensor(const bool constant)
  : SymmElasticityTensor(constant),
    _E1(-1), _E2(-1), _E3(-1),
    _nu12(-1), _nu13(-1), _nu23(-1),
    _G12(-1), _G13(-1), _G23(-1)
{}

void
SymmOrthotropicElasticityTensor::setYoungsModuli(const Real E_1, const Real E_2, const Real E_3)
{
  _E1 = E_1;
  _E2 = E_2;
  _E3 = E_3;
}

void
SymmOrthotropicElasticityTensor::setPoissonsRatios(const Real nu_12, const Real nu_13, const Real nu_23)
{
  _nu12 = nu_12;
  _nu13 = nu_13;
  _nu23 = nu_23;
}

void
SymmOrthotropicElasticityTensor::setShearModuli(const Real G_12, const Real G_13, const Real G_23)
{
  _G12 = G_12;
  _G13 = G_13;
  _G23 = G_23;
}

void
SymmOrthotropicElasticityTensor::calculateEntries(unsigned int /*qp*/)
{
  // see http://de.wikipedia.org/wiki/Orthotropie#Elastizit.C3.A4tsgesetz_f.C3.BCr_3D
  // ATTENTION: exy is in the very final position there, but in the fourth here (as in SymmElasticityTensor.h)
  const Real _nu21 = _E2/_E1*_nu12;
  const Real _nu31 = _E3/_E1*_nu13;
  const Real _nu32 = _E3/_E2*_nu23;
  const Real D = 1. - _nu12*_nu21 - _nu13*_nu31 - _nu23*_nu32 - 2*_nu12*_nu23*_nu31;
  
  const Real C11( (         1. - _nu23*_nu32)/D * _E1 );
  const Real C12( (_nu13*_nu32 + _nu12      )/D * _E2 );
  const Real C13( (_nu12*_nu23 + _nu13      )/D * _E3 );
  const Real C22( (         1. - _nu13*_nu31)/D * _E2 );
  const Real C23( (_nu21*_nu13 + _nu23      )/D * _E3 );
  const Real C33( (         1. - _nu12*_nu21)/D * _E3 );
  const Real C44( _G12 );
  const Real C55( _G23 );
  const Real C66( _G13 );

  /* see SymmElasticityTensor.h:
   *   Entries:                  Indices:
   *   C11 C12 C13 C14 C15 C16     0  1  2  3  4  5
   *       C22 C23 C24 C25 C26        6  7  8  9 10
   *           C33 C34 C35 C36          11 12 13 14
   *               C44 C45 C46             15 16 17
   *                   C55 C56                18 19
   *                       C66                   20
   */
  _val[ 0] = C11;  _val[ 1] = C12;  _val[ 2] = C13;  _val[ 3] =       _val[ 4] =       _val[ 5] = 0;
                   _val[ 6] = C22;  _val[ 7] = C23;  _val[ 8] =       _val[ 9] =       _val[10] = 0;
                                    _val[11] = C33;  _val[12] =       _val[13] =       _val[14] = 0;
                                                     _val[15] = C44;  _val[16] =       _val[17] = 0;
                                                                      _val[18] = C55;  _val[19] = 0;
                                                                                       _val[20] = C66;
}

Real
SymmOrthotropicElasticityTensor::stiffness( const unsigned int i, const unsigned int j,
                                            const RealGradient & test,
                                            const RealGradient & phi )
{
  RealGradient b;
  if (0 == i && 0 == j)
  {
    b(0) = _val[ 0]*phi(0);
    b(1) = _val[15]*phi(1);
    b(2) = _val[20]*phi(2);
  }
  else if (1 == i && 1 == j)
  {
    b(0) = _val[15]*phi(0);
    b(1) = _val[ 6]*phi(1);
    b(2) = _val[18]*phi(2);
  }
  else if (2 == i && 2 == j)
  {
    b(0) = _val[20]*phi(0);
    b(1) = _val[18]*phi(1);
    b(2) = _val[11]*phi(2);
  }
  else if (0 == i && 1 == j)
  {
    b(0) = _val[ 1]*phi(1);
    b(1) = _val[15]*phi(0);
    b(2) = 0;
  }
  else if (1 == i && 0 == j)
  {
    b(0) = _val[15]*phi(1);
    b(1) = _val[ 1]*phi(0);
    b(2) = 0;
  }
  else if (1 == i && 2 == j)
  {
    b(0) = 0;
    b(1) = _val[ 7]*phi(2);
    b(2) = _val[18]*phi(1);
  }
  else if (2 == i && 1 == j)
  {
    b(0) = 0;
    b(1) = _val[18]*phi(2);
    b(2) = _val[ 7]*phi(1);
  }
  else if (0 == i && 2 == j)
  {
    b(0) = _val[ 2]*phi(2);
    b(1) = 0;
    b(2) = _val[20]*phi(0);
  }
  else if (2 == i && 0 == j)
  {
    b(0) = _val[20]*phi(2);
    b(1) = 0;
    b(2) = _val[ 2]*phi(0);
  }
  else
  {
    mooseError( "Wrong index in stiffness calculation" );
  }
  return test * b;
}

void
SymmOrthotropicElasticityTensor::multiply( const SymmTensor & x, SymmTensor & b ) const
{
  const Real xx = x.xx();
  const Real yy = x.yy();
  const Real zz = x.zz();
  const Real xy = x.xy();
  const Real yz = x.yz();
  const Real zx = x.zx();

  b.xx() = _val[ 0]*xx + _val[ 1]*yy + _val[ 2]*zz;
  b.yy() = _val[ 1]*xx + _val[ 6]*yy + _val[ 7]*zz;
  b.zz() = _val[ 2]*xx + _val[ 7]*yy + _val[11]*zz;
  b.xy() = 2*_val[15]*xy;
  b.yz() = 2*_val[18]*yz;
  b.zx() = 2*_val[20]*zx;
}
