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

  params.addCoupledVar("Ta", 0, "The (position dependent) active tension in fibre direction that will finally drive contraction, see (8) in [Whiteley 2007]. Default is Ta=0, i.e. no active tension anywhere.");
  params.addParam<FunctionName>("Ta_function", "A function that describes the (position dependent) active tension in fibre direction that will finally drive contraction, see (8) in [Whiteley 2007].");
  params.addCoupledVar("p", 0, "Hydrostatic pressure that acts as a Lagrange multiplier to ensure incompressibility. Works best with PressureLagrangeMultiplier kernel.");
  return params;
}

CardiacMechanicsMaterial::CardiacMechanicsMaterial(const std::string  & name,
                                                 InputParameters parameters)
  :Material(name, parameters),
   _stress(declareProperty<RealTensorValue>("Kirchhoff_stress")),
   _stress_derivative(declareProperty<SymmGenericElasticityTensor>("Kirchhoff_stress_derivative")),
   _J(declareProperty<Real>("det_displacement_gradient")),
   _W(declareProperty<Real>("elastic_energy_density")),
   _Rf(getMaterialProperty<RealTensorValue>("R_fibre")),
   _Ef(getMaterialProperty<RealVectorValue>("E_fibre")),
   _Es(getMaterialProperty<RealVectorValue>("E_sheet")),
   _En(getMaterialProperty<RealVectorValue>("E_normal")),
   _has_Ta(isCoupled("Ta")),
   _Ta(_has_Ta ? coupledValue("Ta") : _zero),
   _has_Ta_function(isParamValid("Ta_function")),
   _Ta_function( _has_Ta_function ? &getFunction("Ta_function") : NULL ),
   _has_p(isCoupledScalar("p")),
   _p(_has_p ? coupledScalarValue("p") : _zero),
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
  RealTensorValue F((*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
  F(0,0) += 1;
  F(1,1) += 1;
  F(2,2) += 1;
   // ...its determinant is a measure for local volume changes (is needed in kernel that ensures incompressibility via hydrostatic pressure/Lagrange multiplier p)
  _J[_qp] = F.det();
  // From here on, we go over to fibre coordinates, i.e. for C, E, T
  // Cauchy-Green deformation tensor in fibre coordinates: C* = R^T F^T F R
  const SymmTensor C(symmProd(_Rf[_qp], symmProd(F)));
  // Lagrange-Green strain tensor
  const SymmTensor E( (C - _id) * 0.5 );
  // in the rotated system compute _stress[_qp], _stress_derivative[_qp], and _W[_qp]
  computeQpStressProperties(C, E);
  // Add hydrostatic pressure and active tension
  // The following steps render
  //    T asymmetric, i.e. we need a general (non-symmetric) tensor here
  //    D asymmetric wrt. (MN)<->(PQ), i.e. we need the full fourth order tensor here.
  if (_has_p || _has_Ta || _has_Ta_function) {
    // Inverse of the Cauchy Green deformation tensor (note that det(C_fibre) = det(C) = det(F^T)*det(F) = det(F)^2, since det(R)==1 )
    const SymmTensor Cinv( symmInv(C, _J[_qp]*_J[_qp]) );

    // Add hydrostatic pressure as Lagrange multiplier to ensure incompressibility
    if (_has_p) {
      /**
       * @todo TODO: Well, this is a bit tricky. The following line tries to circumvent an issue in
       * Moose's initialization order. ScalarVariables are initialized after the material. Thus, if we use
       * _p in the very first attempt of callng computQPProperties(), this will fail. Since I assume
       * that zero pressure as initial guess is not too bad anyway, we insert it directly in case
       * _p is not yet ready.
       * Just for reference: The current initialization order in Moose is as follows:
       *   1) First initialization of regular nonlinear variables in line 533 of FEProblem.C (call to computeUserObjects() )
       *      via line 167 in ComputeUserObjectsThread.C (call to _fe_problem.reinitElem(elem, _tid) )
       *   2) First call to Material::computeProperties also in line 533 of FEProblem.C (call to computeUserObjects() )
       *      via line 168 in ComputeUserObjectsThread.C ( call to _fe_problem.reinitMaterials(_subdomain, _tid) )
       *   3) [Much later] First initialization of scalar variables in Line 571 of FEProblem.C ( call to reinitScalars() )
       */
      const Real p( _p.size() > 0 ? _p[0] : 0.);

      _stress[_qp] -= STtoRTV( Cinv * p );
      // for the derivative of T, things do become slightly complicated as we have to do
      // _stress_derivative[_qp](MNPQ) += 2 * _p[0] * Cinv(M,P) * Cinv(Q,N)
      // Note that the pressure term is symmetric in Q<->N and in M<->P, i.e. C(MNPQ)=C(MQPN) and C(MNPQ)=C(PNMQ) due to symmetry of Cinv.
      for (int M=0;M<3;M++)
        for (int N=0;N<3;N++)
          for (int P=0;P<3;P++)
            for (int Q=0;Q<3;Q++) {
              _stress_derivative[_qp](M,N,P,Q) = 2 * p * Cinv(M,P) * Cinv(Q,N);
            }
      /// @todo TODO: how does this go into the elastic energy ?
      // _W[_qp] += ??
    }
    // Add active tension in fibre direction
    if (_has_Ta || _has_Ta_function) {
      Real rTa;
      if (_has_Ta)
        rTa = _Ta[_qp];
      else
        rTa = _Ta_function->value(_t, _q_point[_qp]);
      // representation of active tension in fibre direction in outer coordinate system
      const SymmTensor Ta( symmProd(_Rf[_qp].transpose(), SymmTensor(rTa, 0, 0, 0, 0, 0)) );
      _stress[_qp] += prod ( Ta, Cinv );
      // _stress_derivative[_qp](MNPQ) += 2 * _Ta[_qp] delta(M1) delta(N1) * invC(M,P) * invC(Q,N);
      // Ta is a diagonal matrix, i.e. there is a delta(MN) involved
      // furthermore, Cinv is symmetric
      for (int M=0;M<3;M++)
        for (int P=0;P<3;P++)
          for (int Q=0;Q<3;Q++)
            _stress_derivative[_qp](M,M,P,Q) += 2 * Ta(M,M) * Cinv(M,P) * Cinv(Q,M);
      /// @todo TODO: how does this go into the elastic energy ?
      // _W[_qp] += ??
    }
  }

  // rotate back into the outer coordinate system
  _stress[_qp] = _Rf[_qp] * _stress[_qp] * _Rf[_qp].transpose();
  _stress_derivative[_qp] = _stress_derivative[_qp].quadProduct(_Rf[_qp]);
}

