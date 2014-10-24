/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "CardiacPropertiesMaterial.h"

template<>
InputParameters validParams<CardiacPropertiesMaterial>()
{
  InputParameters params = validParams<Material>();
  //params.addRequiredCoupledVar("vmem","Membrane potential needed as input for ion channel model");
  //params.addParam<std::string>("PropagParams", "+Default", "Command line parameters given to propag, e.g. '+F Test.par'. These are handed over to the propag parameter parser and can essentially be the same as used for propag. Many of them are ignored, though. Default: '+Default'");
  return params;
}

CardiacPropertiesMaterial::CardiacPropertiesMaterial(const std::string & name,
                                 InputParameters parameters) :
  Material(name, parameters),
  _Ef(declareProperty<RealVectorValue>("E_fibre")),
  _En(declareProperty<RealVectorValue>("E_normal")),
  _Es(declareProperty<RealVectorValue>("E_sheet")),
  _Rf(declareProperty<RealTensorValue>("R_fibre")),
  _sigma(declareProperty<RealVectorValue>("Sigma")),
  _SubstanceID(-1)
{}



void CardiacPropertiesMaterial::computeQpFibreCoordinates() {
  const RealVectorValue center(0.2, 0.8, 0.0); //! @todo: make this a parameter
  // position of quadrature point that is being processed relative to virtual center
  const RealVectorValue x(_q_point[_qp] - center);
  //! @todo: make these externally definable functions, here they are more or less arbitrarily defined (but still similar to Figure 1 in [Holzapfel 2009]
  const RealVectorValue f90(-x(2),    0.,  x(0)); // fibre direction at the very inner sheeet
  const RealVectorValue f50( x(1), -x(0),    0.); // fibre direction in mid-wall position
  const RealVectorValue f10( x(2),    0., -x(0)); // fibre direction at the outer sheet
  //! @todo: make these parameters
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


void CardiacPropertiesMaterial::computeQpConductivities()
{
  _sigma[_qp] = _Rf[_qp].transpose() * RealVectorValue(_sigmal, _sigmat, _sigmat);
}


void CardiacPropertiesMaterial::computeQpCellInfo()
{
  // every node corresponds to a cell (ionic properties apply to nodes; materials [e.g. conductivity] apply to elements)
  // there are a few properties of the membrane models that can be specified on a per-node basis; this can happen here

  //! @todo This is the place where spatially-dependent models and celltypes can be introduced

}


void CardiacPropertiesMaterial::computeSubstanceProperties()
{
  //! @todo Initialize correct values here
  _sigmat = 0.0;
  _sigmal = 0.0;
}


void CardiacPropertiesMaterial::computeProperties()
{
  // finding out the correct substance index and conductivities only has to be done once here, not individually for every quadrature point
  computeSubstanceProperties();
  Material::computeProperties();
}

void CardiacPropertiesMaterial::computeQpProperties()
{
  computeQpFibreCoordinates();
  computeQpConductivities();
  computeQpCellInfo();
}
