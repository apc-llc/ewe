#include "CardiacFibresMaterial.h"

template<>
InputParameters validParams<CardiacFibresMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addParam<RealTensorValue>("fixed_R", "Externally fixed fibre rotation matrix (unit vectors row-wise: e_f, e_s, e_n). Primarily meant for debugging purposes.");
  params.addCoupledVar("thickness_parameter", "Aux variable with the thickness parameter (normalized mural position), will most often be produced with a CardiacThicknessParameterAux kernel. Make sure that its distinguishLVRV==false.");
  return params;
}

CardiacFibresMaterial::CardiacFibresMaterial(const std::string & name,
                                 InputParameters parameters) :
  Material(name, parameters),
  _Ef(declareProperty<RealVectorValue>("E_fibre")),
  _Es(declareProperty<RealVectorValue>("E_sheet")),
  _En(declareProperty<RealVectorValue>("E_normal")),
  _Rf(declareProperty<RealTensorValue>("R_fibre")),
  _id(RealTensorValue(1,0,0,0,1,0,0,0,1)),
  _has_fixed_R(isParamValid("fixed_R")),
  _fixed_R(_has_fixed_R ? getParam<RealTensorValue>("fixed_R") : _id),
  _has_e(isCoupled("thickness_parameter")),
  _e(_has_e ? coupledValue("thickness_parameter") : _zero),
  _grad_e(_has_e ? coupledGradient("thickness_parameter") : _grad_zero)
{
  if ( !(_has_fixed_R ^ _has_e) )
    mooseError("CardiacFibresMaterial: Either fixed_R or thickness_parameter have to be given (neither both nor none of them).");

  if (_has_fixed_R && _fixed_R.det() != 1) // TODO: det[R]==1 is necessary but not sufficient for R being a rotation matrix
    mooseError("CardiacFibresMaterial: fixed_R has been given externally, but it is not a pure rotation matrix: det[fixed_R] != 1.");
}

void CardiacFibresMaterial::computeQpProperties()
{
  if (_has_fixed_R) {
    _Rf[_qp] = _fixed_R;
    _Ef[_qp] = _Rf[_qp].row(0);
    _Es[_qp] = _Rf[_qp].row(1);
    _En[_qp] = _Rf[_qp].row(2);
  } else if (_has_e) {
    // initial angle for fibre angle as stated by [Potse 2006]
    // We follow the notation from this paper. Our e is defined slightly different, though.
    const Real pi(3.141592653589);
    const Real R( _e[_qp] < 0 ? /* RV */ pi/4. : /* LV */ pi/3.); ///< \todo TODO: we would need to distinguish LV and RV here, but have to use distinguishLVRV==false in the CardiacThicknessParameterAux to prevent _grad_e from being wrong. --> Need another way of distinguishing...
    const Real bracket(2. * _e[_qp] - 1.);
    const Real alpha(R * bracket * bracket * bracket); // We avoid the pow() call here. - Might not make a real performance difference, though.
    // we already know the normal vector's direction (negative because of our e being (1-e) of [Potse 2006])
    _En[_qp] = VectorNormalize( _grad_e[_qp].size()==0 ? RealVectorValue(_q_point[_qp]) : -_grad_e[_qp]); ///< \todo TODO: find a better way of dealing with |grad_e|==0
    // the fibre vector shall be rotated by pi/2-alpha wrt. the z-axis
    RealVectorValue Ef(0., std::cos(alpha), std::sin(alpha));
    // orthogonalization wrt _En[_qp]
    Ef = Ef - (Ef*_En[_qp])*_En[_qp];
    // normalization
    _Ef[_qp] = VectorNormalize(Ef);
    // sheet direction vector
    /// \todo TODO: normalization should not be necessary here as _Ef and _En are orthonormal
    _Es[_qp] = VectorNormalize(VectorProduct(_Ef[_qp], _En[_qp]) /* stupid way of constructing the missing orthogonal vector*/);
    // row-wise initialization of _Rf
    _Rf[_qp] = RealTensorValue(_Ef[_qp], _Es[_qp], _En[_qp]);
  } else {
    mooseError("CardiacFibresMaterial: should not be here");
  }
}
