#include "CardiacMechanicsMaterial.h"
#include "ColumnMajorMatrix.h"
#include "CardiacSolidMechanicsMaterial.h"
#include "SymmOrthotropicElasticityTensor.h"
#include "VolumetricModel.h"
#include "TensorHelpers.h"

using namespace TensorHelpers;

template<>
InputParameters validParams<CardiacMechanicsMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredCoupledVar("displacements", "The x, y, and z displacement");

  params.addCoupledVar("active_tension", "The (position dependent) active tension in fibre direction that will finally drive contraction, see (8) in [Whiteley, 2007]. Unit: kPa. Default is Ta=0, i.e. no active tension anywhere.");
  params.addParam<FunctionName>("active_tension_function", "A function that describes the (position dependent) active tension in fibre direction that will finally drive contraction, see (8) in [Whiteley, 2007]. Unit: kPa.");
  params.addCoupledVar("hydrostatic_pressure", "Hydrostatic pressure that acts as a Lagrange multiplier to ensure incompressibility. Unit: kPa. Works best with CardiacMechanicsIncompressibilityLagrangeMultiplier kernel. Default: 0. (no hydrostatic pressure).");
  params.set<bool>("use_displaced_mesh") = false;

  // we restrict output to avoid warnings about KirchhoffStress being impossible to be used in output
  std::vector<std::string> output_properties;
  //output_properties.push_back("displacement_gradient");
  output_properties.push_back("det_displacement_gradient");
  params.set<std::vector<std::string> >("output_properties") = output_properties;

  return params;
}

CardiacMechanicsMaterial::CardiacMechanicsMaterial(const std::string  & name,
                                                 InputParameters parameters)
  :Material(name, parameters),
   _stress(declareProperty<SymmTensor>("Kirchhoff_stress")),
   _stress_derivative(declareProperty<CardiacElasticityTensor>("Kirchhoff_stress_derivative")),
   _F(declareProperty<RealTensorValue>("displacement_gradient")),
   _J(declareProperty<Real>("det_displacement_gradient")),
   _Cinv(declareProperty<SymmTensor>("Cinv")),
   _Finv(declareProperty<RealTensorValue>("inv_displacement_gradient")),
   _W(declareProperty<Real>("elastic_energy_density")),
   _Rf(getMaterialProperty<RealTensorValue>("R_fibre")),
   _Ef(getMaterialProperty<RealVectorValue>("E_fibre")),
   _Es(getMaterialProperty<RealVectorValue>("E_sheet")),
   _En(getMaterialProperty<RealVectorValue>("E_normal")),
   _has_Ta(isCoupled("active_tension")),
   _Ta(_has_Ta ? coupledValue("active_tension") : _zero),
   _has_Ta_function(isParamValid("active_tension_function")),
   _Ta_function( _has_Ta_function ? &getFunction("active_tension_function") : NULL ),
   _has_p(isCoupled("hydrostatic_pressure")),
   _p(_has_p ? coupledValue("hydrostatic_pressure") : _zero),
   _id(scaledID(1))
{
  if (_has_Ta && _has_Ta_function)
    mooseError("CardiacMechanicsMaterial: Only Ta or Ta_function may be given, not both of them.");

  // see http://mooseframework.org/wiki/Faq/#coupling-to-an-arbitrary-number-of-variables-back-to-top for details on this magic
  _grad_disp.resize(coupledComponents("displacements"));

  mooseAssert(_grad_disp.size() == 3, "CardiacMechanicsMaterial: displacements must have exactly 3 components");

  for (unsigned int i=0; i<_grad_disp.size(); ++i)
    _grad_disp[i] = &coupledGradient("displacements", i);
}

void
CardiacMechanicsMaterial::computeQpProperties()
{
  // local deformation gradient tensor: F(ij) = dx(i)/dX(j)
  // Note that the nonlinear variables are displacements u(i)=x(i)-X(i), thus dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
  _F[_qp] = RealTensorValue((*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
  _F[_qp](0,0) += 1;
  _F[_qp](1,1) += 1;
  _F[_qp](2,2) += 1;
  // ...its determinant is a measure for local volume changes (is needed in kernel that ensures incompressibility via hydrostatic pressure/Lagrange multiplier p)
  _J[_qp] = _F[_qp].det();
  // ...its inverse is needed for the Jacobian of the incompressibility PDE and for boundary conditions in the deformed configuration
  _Finv[_qp] = mat_inv(_F[_qp], _J[_qp]);
  // From here on, we go over to fibre coordinates, i.e. for C, E, T
  // Cauchy-Green deformation tensor in fibre coordinates: C* = R^T F^T F R
  const SymmTensor C(symmProd(_Rf[_qp], symmProd(_F[_qp])));
  // Lagrange-Green strain tensor
  const SymmTensor E( (C - _id) * 0.5 );
  // in the rotated system compute _stress[_qp], _stress_derivative[_qp], and _W[_qp]
  computeQpStressProperties(C, E);
  // Add hydrostatic pressure and active tension
  if (_has_p || _has_Ta || _has_Ta_function) {
    // Inverse of the Cauchy Green deformation tensor (note that det(C_fibre) = det(C) = det(F^T)*det(F) = det(F)^2, since det(R)==1 )
    const SymmTensor Cinv( symmInv(C, _J[_qp]*_J[_qp]) );

    // Add hydrostatic pressure as Lagrange multiplier to ensure incompressibility
    if (_has_p) {
      // _stress(MN) += p*Cinv(MN)
      _stress[_qp] -= Cinv * _J[_qp] * _p[_qp];
      // for the derivative of T, things do become slightly complicated as we have to do
      // _stress_derivative(MNPQ) += 2 * p * Cinv(M,P) * Cinv(Q,N)
      CardiacElasticityTensor sdp;
                                 /* fancy lambda function syntax makes things much easier here */
      sdp.fill_from_minor_iter( [&](const unsigned int M,
                                    const unsigned int N,
                                    const unsigned int P,
                                    const unsigned int Q) -> Real { return _p[_qp] * _J[_qp] * (2.*Cinv(M,P)*Cinv(Q,N) - Cinv(Q,P)*Cinv(M,N)); } );

      _stress_derivative[_qp] += sdp;
      // no energy contribution from a Lagrange multiplier
      // _W[_qp] += 0

      // the inverse of C in outer coordinates is required for the kernel's Jacobian matrix element wrt. pressure
      _Cinv[_qp] = symmProd(_Rf[_qp].transpose(), Cinv);
    }

    // Add active tension in fibre direction
    if (_has_Ta || _has_Ta_function) {
      Real Ta;
      if (_has_Ta)
        Ta = _Ta[_qp];
      else
        Ta = _Ta_function->value(_t, _q_point[_qp]);

      // _stress(MN) += _Ta * delta(M1) delta(N1) * invC(M,N)
      const int M(0);
      const int N(0);
      _stress[_qp](M,N) += Ta*Cinv(M,N);
      // _stress_derivative(MNPQ) += -2 * _Ta * delta(M1) delta(N1) * invC(M,P) * invC(Q,N);
      CardiacElasticityTensor sda;
      for (int P=0;P<3;P++)
        for (int Q=P;Q<3;Q++)
          sda(M,N,P,Q) = -2. * Ta * Cinv(M,P) * Cinv(Q,N);

      _stress_derivative[_qp] += sda;
      /// @todo TODO: how does this go into the elastic energy ?
      // _W[_qp] += ??
    }
  }

  // rotate back into the outer coordinate system
  _stress[_qp] = symmProd(_Rf[_qp].transpose(), _stress[_qp]);
  _stress_derivative[_qp] = _stress_derivative[_qp].quadProduct(_Rf[_qp]);
}

