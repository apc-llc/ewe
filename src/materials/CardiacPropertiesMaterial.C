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
  params.addParam<std::string>("PropagParams", "+Default", "Command line parameters given to propag, e.g. '+F Test.par'. These are handed over to the propag parameter parser and can essentially be the same as used for propag. Many of them are ignored, though. Default: '+Default'");
  return params;
}

CardiacPropertiesMaterial::CardiacPropertiesMaterial(const std::string & name,
                                 InputParameters parameters) :
  Material(name, parameters),
  _cell_info(declareProperty<Membrane_cell_info>("cell_info")),
  _Rf(getMaterialProperty<RealTensorValue>("R_fibre")),
  _sigma(declareProperty<RealVectorValue>("Sigma")),
  _SubstanceID(-1),
  _PropagParams(getParam<std::string>("PropagParams"))
{
  // TODO: Moose creates three material objects: volume, neighbor and boundary for every thread
  // to only initialize propag once (see lines 1580ff in FEProblem.C)
  // we call the init routines only for the volume part of the very first thread.
  // NOTE: This assumes that volume is ALWAYS created first...
  // TODO: I would really not assume that any propag-routines are thread-safe.
  if (_tid == 0 && !_bnd && !_neighbor) {
    
    std::string paramstring = reduce("execname " + _PropagParams, " ");
    std::vector<char> paramchars(paramstring.begin(), paramstring.end());
    std::vector<char*> argv = make_argv(paramchars);
    int argc = argv.size();
    
    int s;
    do{
      s= param(PARAMETERS,&argc, &argv[0]);
      if(s==PrMERROR||s==PrMFATAL)
      {
        Error(1,"Error reading parameters");
      }
      else if(s==PrMQUIT)
      {
        fprintf(stderr,"\n*** Quitting by user's request\n\n");
        exit(0);
      }
    }while(s==PrMERROR);
    
    ion_info();
    // create ionic mapping (colors to ion models)
    ion_setup_nmap();
    // creates substances/materials, each with different connectivity G
    setup_substances();
    
    ion_init(_dt); // this simply initializes all loaded ion models - we do not check wether they are actually used
  }
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
  computeQpConductivities();
  computeQpCellInfo();
}
