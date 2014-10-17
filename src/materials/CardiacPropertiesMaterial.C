/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/

#include "CardiacPropertiesMaterial.h"
#include "propag_d.h"
#include "anatomy.h"

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
  _cell_info(declareProperty<Membrane_cell_info>("cell_info")),
  _Ef(declareProperty<RealVectorValue>("E_fibre")),
  _En(declareProperty<RealVectorValue>("E_normal")),
  _Es(declareProperty<RealVectorValue>("E_sheet")),
  _Rf(declareProperty<RealTensorValue>("R_fibre")),
  _sigma(declareProperty<RealVectorValue>("Sigma")),
  _SubstanceID(-1)
{}



void CardiacPropertiesMaterial::computeQpFibreCoordinates() {
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


void CardiacPropertiesMaterial::computeQpConductivities()
{
  _sigma[_qp] = _Rf[_qp].transpose() * RealVectorValue(_sigmal, _sigmat, _sigmat);
}


void CardiacPropertiesMaterial::computeQpCellInfo()
{
  // every node corresponds to a cell (ionic properties apply to nodes; materials [e.g. conductivity] apply to elements)
  // there are a few properties of the membrane models that can be specified on a per-node basis; this can happen here

  // TODO: This is the place where spatially-dependent models and celltypes can be introduced
  // The following is bernus as long as only a single model is loaded
  // among other places, you should take a look at setup_substances() in anatomy.c, bernus_p.dep and ion_setup_cell_info() in ion.c for handling of params
  _cell_info[_qp].mcode = 1;           // membrane model code -- BERNUS
  _cell_info[_qp].ccode = BERNUS_EPIC; // membrane model-specific cell type code --> BERNUS_EPIC
  _cell_info[_qp].param = NULL;        // invalid to avoid accidential use; originally, param = malloc((size_t)(ion_mi[mc].Nparam)*sizeof(float)), instead of the allocation magic, we should better just point to some global storage of parameter sets here...
}


void CardiacPropertiesMaterial::computeSubstanceProperties()
{
  char* substancename = "ventricle"; // TODO: make this a parameter
  int cdom(DOM_INTRA); // TODO: make this a parameter

  for (int i=0;i<200;i++)
    if(strcmp(prm_substance[i].name, substancename ) !=0 ) {
      _SubstanceID = i;
      break;
    }
  if (_SubstanceID == -1) Error(42,"Substance %s not found.\n", substancename);
  
  // see conductivites() in fibres.c
  #define gbulk(i,e) ((i) +(e) )
  #define gmono(i,e) ((i) +(e) ==0?0.0:(i) *(e) /((i) +(e) ) )
  switch (cdom) {
    case DOM_INTRA:
      _sigmat = prm_substance[_SubstanceID].sigma_it;
      _sigmal = prm_substance[_SubstanceID].sigma_il;
      break;
    case DOM_EXTRA:
      _sigmat = prm_substance[_SubstanceID].sigma_et;
      _sigmal = prm_substance[_SubstanceID].sigma_el;
      break;
    case DOM_MONO:
      _sigmat = gmono(prm_substance[_SubstanceID].sigma_it,prm_substance[_SubstanceID].sigma_et);
      _sigmal = gmono(prm_substance[_SubstanceID].sigma_il,prm_substance[_SubstanceID].sigma_el);
      break;
    case DOM_BULK:
      _sigmat = gbulk(prm_substance[_SubstanceID].sigma_it,prm_substance[_SubstanceID].sigma_et);
      _sigmal = gbulk(prm_substance[_SubstanceID].sigma_il,prm_substance[_SubstanceID].sigma_el);
      break;
    default:
      Error(22,"This cannot happen: unknown domain=%d\n", cdom);
  }
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
