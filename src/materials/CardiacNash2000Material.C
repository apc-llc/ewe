/****************************************************************/
/*                                                              */
/*  Implementation of Cardiac Tissue elasticity as given in     */
/*  [Nash2000]                                              */
/*                                                              */
/****************************************************************/

#include "CardiacNash2000Material.h"
#include "ColumnMajorMatrix.h"
#include "CardiacSolidMechanicsMaterial.h"
#include "SymmOrthotropicElasticityTensor.h"
#include "VolumetricModel.h"
#include "TensorHelpers.h"

using namespace TensorHelpers;

template<>
InputParameters validParams<CardiacNash2000Material>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredCoupledVar("dispx", "The x displacement");
  params.addRequiredCoupledVar("dispy", "The y displacement");
  params.addRequiredCoupledVar("dispz", "The z displacement");

  params.addRequiredParam<std::vector<Real> >("k_MN", "Material parameters k_MN in following order: k_11, k_22, k_33, k_12, k_23, k_13");
  params.addRequiredParam<std::vector<Real> >("a_MN", "Material parameters a_MN in following order: a_11, a_22, a_33, a_12, a_23, a_13");
  params.addRequiredParam<std::vector<Real> >("b_MN", "Material parameters b_MN in following order: b_11, b_22, b_33, b_12, b_23, b_13");

  params.addCoupledVar("Ta", 0, "The (position dependent) active tension in fibre direction that will finally drive contraction, see (8) in [Whiteley 2007]. Default is Ta=0, i.e. no active tension anywhere.");
  params.addParam<FunctionName>("Ta_function", "A function that describes the (position dependent) active tension in fibre direction that will finally drive contraction, see (8) in [Whiteley 2007].");
  params.addCoupledVar("p", 0, "Hydrostatic pressure that acts as a Lagrange multiplier to ensure incompressibility. Works best with PressureLagrangeMultiplier kernel.");
  return params;
}

CardiacNash2000Material::CardiacNash2000Material(const std::string  & name,
                                                 InputParameters parameters)
  :Material(name, parameters),
   _grad_dispx(coupledGradient("dispx")),
   _grad_dispy(coupledGradient("dispy")),
   _grad_dispz(coupledGradient("dispz")),
   _k(SymmTensor(getParam<std::vector<Real> >("k_MN"))),
   _a(SymmTensor(getParam<std::vector<Real> >("a_MN"))),
   _b(SymmTensor(getParam<std::vector<Real> >("b_MN"))),
   _stress(declareProperty<RealTensorValue>("Kirchhoff_stress")),
   _stress_derivative(declareProperty<SymmGenericElasticityTensor>("Kirchhoff_stress_derivative")),
   _F(declareProperty<RealTensorValue>("displacement_gradient")),
   _J(declareProperty<Real>("det_displacement_gradient")),
   _W(declareProperty<Real>("elastic_energy_density")),
   _Rf(getMaterialProperty<RealTensorValue>("R_fibre")),
   _has_Ta(isCoupled("Ta")),
   _Ta(_has_Ta ? coupledValue("Ta") : _zero),
   _has_Ta_function(isParamValid("Ta_function")),
   _Ta_function( _has_Ta_function ? &getFunction("Ta_function") : NULL ),
   _has_p(isCoupledScalar("p")),
   _p(_has_p ? coupledScalarValue("p") : _zero),
   _id(1, 1, 1, 0, 0, 0)
{
  if (_has_Ta && _has_Ta_function)
    mooseError("CardiacNash2000Material: Only Ta or Ta_function may be given, not both of them.");
}

void
CardiacNash2000Material::computeQpProperties()
{
  // local deformation gradient tensor: F(ij) = dx(i)/dX(j)
  // Note that the nonlinear variables are displacements u(i)=x(i)-X(i), thus dx(i)/dX(j) = du(i)/dX(j) + delta(ij)
  _F[_qp] = RealTensorValue(_grad_dispx[_qp], _grad_dispy[_qp], _grad_dispz[_qp]);
  _F[_qp](0,0) += 1;
  _F[_qp](1,1) += 1;
  _F[_qp](2,2) += 1;
   // ...its determinant is a measure for local volume changes (is needed in kernel that ensures incompressibility via hydrostatic pressure/Lagrange multiplier p)
  _J[_qp] = _F[_qp].det();
  // From here on, we go over to fibre coordinates, i.e. for C, E, T
  // Cauchy-Green deformation tensor in fibre coordinates: C* = R^T F^T F R
  const SymmTensor C(symmProd(_Rf[_qp], symmProd(_F[_qp])));
  // Lagrange-Green strain tensor
  const SymmTensor E( (C - _id) * 0.5 );
  
  // 2nd Piola-Kirchhoff stress tensor: T(MN) = 1/2[dW/dE(MN) + dW/dE(NM)] + [- p + Ta delta(M1) delta(N1) ] C^-1(MN)
  // We make use of dW/dE(MN) == dW/dE(NM) and will add active tension and pressure terms later
  SymmTensor T;
  // Derivative of T: D(MNPQ) = dT(MN)/dE(PQ)
  // We make use of the symmetry of T and the fact that for our W, dT(MN)/dE(PQ) \propto delta(MP)delta(PQ) if p and Ta are omitted.
  // Thus, a symmetric second-order tensor is sufficient here. Ta and p will be added later, again.
  SymmTensor D;
  // We will sum up the elastic energy contributions inside the loop
  _W[_qp] = 0;

  for (int M=0;M<3;M++)
    for (int N=M;N<3;N++)
    {
      const Real k(_k(M,N));
      const Real e(E(M,N));

      if (k==0) { // summand in W does not contribute at all
        T(M,N) = 0.;
        D(M,N) = 0.;
        _W[_qp] += 0.;
      } else if (k<0) {
        // negative k values force a fallback to the linear case
        T(M,N) = -k;
        D(M,N) =  0.;
        _W[_qp] += -k*e;
      } else /* k>0 */ {
        const Real a(_a(M,N));
        const Real b(_b(M,N));
        const Real d( a - e );
        if (d <= 0)
          mooseError("CardiacNash2000Material: E_{MN} >= a_{MN} - the strain is too large for this model");
        const Real f( b*e/d );
        const Real g( k*pow(d,-b) );

        T(M,N) = g * e * ( 2+f );
        D(M,N) = g * ( 2 + (4+e/d+f)*f );

        _W[_qp] += (M==N ? 1. : 2. ) * g*e*e;
      }
    }

  // Add hydrostatic pressure and active tension
  // The following steps render
  //    T asymmetric, i.e. we need a general (non-symmetric) tensor here
  //    D asymmetric wrt. (MN)<->(PQ), i.e. we need the full fourth order tensor here.
  _stress[_qp] = STtoRTV(T);
  _stress_derivative[_qp] = STtoSGET(D);

  if (_has_p || _has_Ta || _has_Ta_function) {
    // Inverse of the Cauchy Green deformation tensor (note that det(C_fibre) = det(C) = det(F^T)*det(F) = det(F)^2, since det(R)==1 )
    const SymmTensor Cinv( symmInv(C, _J[_qp]*_J[_qp]) );

    // Add hydrostatic pressure as Lagrange multiplier to ensure incompressibility
    if (_has_p) {
      /**
       * \todo TODO: Well, this is a bit tricky. The following line tries to circumvent an issue in
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
          for (int P=M;P<3;P++)
            for (int Q=N;Q<3;Q++) {
              const Real Tp(2 * p * Cinv(M,P) * Cinv(Q,N));
              _stress_derivative[_qp](M,N,P,Q) += Tp;
              _stress_derivative[_qp](N,M,P,Q) += Tp;
              _stress_derivative[_qp](M,N,Q,P) += Tp;
              _stress_derivative[_qp](N,M,Q,P) += Tp;
            }
      /// \todo TODO: how does this go into the elastic energy ?
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
      /// \todo TODO: how does this go into the elastic energy ?
      // _W[_qp] += ??
    }
  }

  // rotate back into the outer coordinate system
  _stress[_qp] = _Rf[_qp] * _stress[_qp] * _Rf[_qp].transpose();
  _stress_derivative[_qp] = _stress_derivative[_qp].quadProduct(_Rf[_qp], _Rf[_qp].transpose(), _Rf[_qp], _Rf[_qp].transpose());
}

