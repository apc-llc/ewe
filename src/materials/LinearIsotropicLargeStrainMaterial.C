#include "LinearIsotropicLargeStrainMaterial.h"
#include "ColumnMajorMatrix.h"
#include "SolidMechanicsMaterial.h"
#include "SymmIsotropicElasticityTensor.h"
#include "VolumetricModel.h"

template<>
InputParameters validParams<LinearIsotropicLargeStrainMaterial>()
{
  InputParameters params = validParams<SolidMechanicsMaterial>();
  params.addRequiredParam<Real>("youngs_modulus", "Young's Modulus");
  params.addRequiredParam<Real>("poissons_ratio", "Poisson's Ratio");
  params.addParam<Real>("t_ref", 0.0, "The reference temperature at which this material has zero strain.");
  params.addParam<Real>("thermal_expansion", 0.0, "The thermal expansion coefficient.");
  params.addParam<bool>("include_large_strain", true, "If set to true, quadratic terms in the displacement are included into the strain otherwise they are omitted (see eq. (3.3.6) in [Belytschko: 'Nonlinear Finite Elements for Continua and Structures', pg. 93].");
  return params;
}

LinearIsotropicLargeStrainMaterial::LinearIsotropicLargeStrainMaterial(const std::string  & name,
                                                 InputParameters parameters)
  :SolidMechanicsMaterial(name, parameters),
   _youngs_modulus(getParam<Real>("youngs_modulus")),
   _poissons_ratio(getParam<Real>("poissons_ratio")),
   _large_strain(getParam<bool>("include_large_strain")),
   _t_ref(getParam<Real>("t_ref")),
   _alpha(getParam<Real>("thermal_expansion")),
   _local_elasticity_tensor(NULL)
{
  SymmIsotropicElasticityTensor * iso_elasticity_tensor = new SymmIsotropicElasticityTensor;
  iso_elasticity_tensor->setYoungsModulus(_youngs_modulus);
  iso_elasticity_tensor->setPoissonsRatio(_poissons_ratio);

  _local_elasticity_tensor = iso_elasticity_tensor;

  _pi = 3.14159;
  _tol = 1.0e-5;


}

LinearIsotropicLargeStrainMaterial::~LinearIsotropicLargeStrainMaterial()
{
  delete _local_elasticity_tensor;
}

void
LinearIsotropicLargeStrainMaterial::computeProperties()
{
  for (_qp=0; _qp < _qrule->n_points(); ++_qp)
  {
      Real alpha = computeAlpha();

    _local_elasticity_tensor->calculate(_qp);

    _elasticity_tensor[_qp] = *_local_elasticity_tensor;


    SymmTensor strn( _grad_disp_x[_qp](0),
                     _grad_disp_y[_qp](1),
                     _grad_disp_z[_qp](2),
                     0.5*(_grad_disp_x[_qp](1)+_grad_disp_y[_qp](0)),
                     0.5*(_grad_disp_y[_qp](2)+_grad_disp_z[_qp](1)),
                     0.5*(_grad_disp_z[_qp](0)+_grad_disp_x[_qp](2)) );
    if (_large_strain)
    {
      strn.xx() += 0.5*(_grad_disp_x[_qp](0)*_grad_disp_x[_qp](0) +
                        _grad_disp_y[_qp](0)*_grad_disp_y[_qp](0) +
                        _grad_disp_z[_qp](0)*_grad_disp_z[_qp](0));
      strn.yy() += 0.5*(_grad_disp_x[_qp](1)*_grad_disp_x[_qp](1) +
                        _grad_disp_y[_qp](1)*_grad_disp_y[_qp](1) +
                        _grad_disp_z[_qp](1)*_grad_disp_z[_qp](1));
      strn.zz() += 0.5*(_grad_disp_x[_qp](2)*_grad_disp_x[_qp](2) +
                        _grad_disp_y[_qp](2)*_grad_disp_y[_qp](2) +
                        _grad_disp_z[_qp](2)*_grad_disp_z[_qp](2));
      strn.xy() += 0.5*(_grad_disp_x[_qp](0)*_grad_disp_x[_qp](1) +
                        _grad_disp_y[_qp](0)*_grad_disp_y[_qp](1) +
                        _grad_disp_z[_qp](0)*_grad_disp_z[_qp](1));
      strn.yz() += 0.5*(_grad_disp_x[_qp](1)*_grad_disp_x[_qp](2) +
                        _grad_disp_y[_qp](1)*_grad_disp_y[_qp](2) +
                        _grad_disp_z[_qp](1)*_grad_disp_z[_qp](2));
      strn.zx() += 0.5*(_grad_disp_x[_qp](2)*_grad_disp_x[_qp](0) +
                        _grad_disp_y[_qp](2)*_grad_disp_y[_qp](0) +
                        _grad_disp_z[_qp](2)*_grad_disp_z[_qp](0));
    }

    // Add in Isotropic Thermal Strain
    if (_has_temp)
    {
      Real isotropic_strain = alpha * (_temp[_qp] - _t_ref);

      strn.addDiag( -isotropic_strain );

      _d_strain_dT.zero();
      _d_strain_dT.addDiag( -alpha );

    }

    SymmTensor v_strain(0);
    SymmTensor dv_strain_dT(0);
    for (unsigned int i(0); i < _volumetric_models.size(); ++i)
    {
      _volumetric_models[i]->modifyStrain(_qp, 1, v_strain, dv_strain_dT);
    }
    SymmTensor strain( v_strain );
    strain *= _dt;
    strain += strn;

    dv_strain_dT *= _dt;
    _d_strain_dT += dv_strain_dT;

    computeStress(strain, _stress[_qp]);

  }
}

void
LinearIsotropicLargeStrainMaterial::computeStress(const SymmTensor & strain,
                                       SymmTensor & stress)
{
  // Add in any extra strain components
  SymmTensor elastic_strain;

  computeStrain(strain, elastic_strain);


  // Multiplier that zeros out stiffness
  //Real h = (1.0 - _c[_qp]*_c[_qp]);
  /*Real vl = -0.05 + _c[_qp]*1.1;
  if (vl < 0.0)
    vl = 0.0;
  if (vl > 1.0)
  vl = 1.0;*/

  Real h = (1.0 + std::cos(_pi*_c[_qp]))/2.0;
  if (h < _tol)
    h = _tol;

  // Save that off as the elastic strain
  _elastic_strain[_qp] = elastic_strain;


  // Create column vector
  // C * e
  stress = (*_local_elasticity_tensor)*h*elastic_strain;

}

void
LinearIsotropicLargeStrainMaterial::computeStrain(const SymmTensor & total_strain, SymmTensor & elastic_strain)
{

  // Multiplier that zeros out stiffness
  //Real h = (1.0 - _c[_qp]*_c[_qp]);
  /*Real vl = -0.05 + _c[_qp]*1.1;
  if (vl < 0.0)
    vl = 0.0;
  if (vl > 1.0)
  vl = 1.0;*/

  Real h = (1.0 + std::cos(_pi*_c[_qp]))/2.0;
  if (h < _tol)
    h = _tol;


  elastic_strain = total_strain;
  //Jacobian multiplier of the stress
  _Jacobian_mult[_qp] = *_local_elasticity_tensor*h;

  SymmTensor d_stress_dT( *_local_elasticity_tensor * _d_strain_dT );
//   d_stress_dT *= _dt;
  d_stress_dT *= h;
  _d_stress_dT[_qp] = d_stress_dT;


}

Real
LinearIsotropicLargeStrainMaterial::computeAlpha()
{
  return _alpha;
}
