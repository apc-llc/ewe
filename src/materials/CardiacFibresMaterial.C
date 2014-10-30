/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "CardiacFibresMaterial.h"

template<>
InputParameters validParams<CardiacFibresMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addParam<RealTensorValue>("fixed_R", "Externally fixed fibre rotation matrix. Primarily meant for debugging purposes.");
  return params;
}

CardiacFibresMaterial::CardiacFibresMaterial(const std::string & name,
                                 InputParameters parameters) :
  Material(name, parameters),
  _Ef(declareProperty<RealVectorValue>("E_fibre")),
  _En(declareProperty<RealVectorValue>("E_normal")),
  _Es(declareProperty<RealVectorValue>("E_sheet")),
  _Rf(declareProperty<RealTensorValue>("R_fibre")),
  _id(RealTensorValue(1,0,0,0,1,0,0,0,1)),
  _has_fixed_R(isParamValid("fixed_R")),
  _fixed_R(_has_fixed_R ? getParam<RealTensorValue>("fixed_R") : _id)
{
  if (_has_fixed_R && _fixed_R.det() != 1)
    mooseError("CardiacFibresMaterial: dixed_R has been given externally, but it is not a pure rotation matrix: det[fixed_R] != 1");
}

void CardiacFibresMaterial::computeQpProperties()
{
  if (_has_fixed_R) {
    _Rf[_qp] = _fixed_R;
    _Ef[_qp] = RealVectorValue(_fixed_R(0,0), _fixed_R(1,0), _fixed_R(2,0));
    _Es[_qp] = RealVectorValue(_fixed_R(0,1), _fixed_R(1,1), _fixed_R(2,1));
    _En[_qp] = RealVectorValue(_fixed_R(0,2), _fixed_R(1,2), _fixed_R(2,2));
  } else {
    const RealVectorValue center(0.2, 0.8, 0.0); // TODO: make this a parameter
    // position of quadrature point that is being processed relative to virtual center
    const RealVectorValue x(_q_point[_qp] - center);
    // TODO: make these externally definable functions, here they are more or less arbitrarily defined (but still similar to Figure 1 in [Holzapfel 2009]
    const RealVectorValue f90(-x(2),    0.,  x(0)); // fibre direction at the very inner sheeet
    const RealVectorValue f50( x(1), -x(0),    0.); // fibre direction in mid-wall position
    const RealVectorValue f10( x(2),    0., -x(0)); // fibre direction at the outer sheet
    // TODO: make these parameters
    const Real r_inner(0.1); // position (radial distance) of the innermost sheeth
    const Real r_outer(0.3); // position (radial distance) of the outermost sheeth
    // radial distance of current quadrature point from centre
    const Real r(x.size());
    
    if (r <= r_inner)
      _Ef[_qp] = f90;
    else if (r >= r_outer )
      _Ef[_qp] = f10;
    else {
      const Real A = (r - r_inner)/(r_outer -r_inner);
      _Ef[_qp] =  (1-2*A)*(1-A)*f90 + 4*A*(1-A)*f50 + A*(2*A-1)*f10;
    }

    _Ef[_qp] = VectorNormalize(_Ef[_qp]);
    _En[_qp] = VectorNormalize(x); // The normal just points outwards as it would do for a sphere. This is not quite exact, but I do not think that we can use _normals[_qp] here as this presumably corresponds to the current facelets normal not the overall body surface normal (which would essentially be undefined inside the tissue)
    _Es[_qp] = VectorNormalize(VectorProduct(_Ef[_qp], _En[_qp]) /* stupid way of constructing the missing orthogonal vector*/);

    _Rf[_qp] = RealTensorValue(_Ef[_qp], _Es[_qp], _En[_qp]).transpose();
  }
}
