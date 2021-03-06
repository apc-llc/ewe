#include "Electrocardio.h"
//#include "CudaBernus.cu"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>

#define dngates 5

template<>
InputParameters validParams<Electrocardio>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredCoupledVar("vmem","Membrane potential needed as input for ion channel model. Unit: mV.");
  //! @todo: For ion channel models that need the diffusion current, have to fetch the value of Imem somehow

  // we restrict output to Imem to avoid warnings about gates being impossible to be used in output
  std::vector<std::string> output_properties;
  output_properties.push_back("Iion");
  params.set<std::vector<std::string> >("output_properties") = output_properties;
  return params;
}

Electrocardio::Electrocardio(const std::string & name,
                                 InputParameters parameters) :
  Material(name, parameters),
  _Iion(declareProperty<Real>("Iion")),
  _gates(declareProperty<std::vector<Real> >("gates")),
  _gates_old(declarePropertyOld<std::vector<Real> >("gates")),
  // coupled variables
  _vmem(coupledValue("vmem"))
{
  
  // Create pointer to a Bernus model object using the factory class
  //_ionmodel = CudaIionmodelFactory::factory(CudaIionmodelFactory::BERNUS);
  _ionmodel = IionmodelFactory::factory(IionmodelFactory::BERNUS);
}

void
Electrocardio::initQpStatefulProperties()
{
  // initialize local gate variable vector
  // std::vector<std::vector<double> > gates(_qrule->n_points(), std::vector<double>(ngates));
  // for(int i = 0 ; i< _qrule->n_points(); i++){
  //   gates[i] = _gates[i];
  // }
  // _ionmodel->initialize(&gates, _qrule->n_points());
}


/*TODO 
  1. port the computeProperties() on GPUs 
  2. merge correctly into the moose framework
*/
//for (unsigned int qp = 0; qp < _qrule->n_points(); ++qp)
void Electrocardio::computeProperties()
{
  int qpsize = _qrule->n_points();
  // std::vector<double> vmem(_qrule->n_points());
  // std::vector<double> Iion(_qrule->n_points());
  // std::vector<std::vector<double> > gates_old(_qrule->n_points(), std::vector<double>(ngates));
  // std::vector<std::vector<double> > gates(_qrule->n_points(), std::vector<double>(ngates));
  
  // for(int i = 0 ; i< vmem.size(); i++){
  //   vmem[i] = _vmem[i];
  //   Iion[i] = _Iion[i];
  //   gates_old[i] = _gates_old[i];
  //   gates[i] = _gates[i];
  // }

  double * vmem, * Iion, * gates_old, * gates;

  gates = (double *)malloc( (dngates) * qpsize  * sizeof(double));
  gates_old = (double *)malloc( (dngates) * qpsize  * sizeof(double));
  vmem = (double *)malloc( qpsize  * sizeof(double));
  Iion = (double *)malloc( qpsize  * sizeof(double));
  

  for(int i = 0; i < qpsize; i++ ){
    vmem[i] = _vmem[i];
    Iion[i] = _Iion[i];
    for(int j = 0; j < dngates;j++){
      gates_old[i * dngates + j ] = _gates_old[i][j];
      gates[i * dngates + j ] = _gates[i][j];
    }
  }

  // _ionmodel->initialize(gates, _qrule->n_points());

  // _ionmodel->rush_larsen_ionforcing(_dt, vmem, gates_old, gates, Iion, _qrule->n_points());

  // transform the data as double * back to moose format

  for(int i = 0 ; i< qpsize; i++){
    _vmem[i] = vmem[i];
    _Iion[i] = Iion[i];
    for(int j = 0; j < dngates;j++){
      _gates_old[i][j] = gates_old[i * dngates + j];
      _gates[i][j] = gates[i * dngates + j];
    }
  }
}


/**
 * @todo documentation
 */
void
Electrocardio::computeQpProperties()
{
  
  // Compute ionforcing
  _Iion[_qp] = _ionmodel->ionforcing(_vmem[_qp], &(_gates_old[_qp]));

  // Copy old values into _gates as initial value
  for (int i=0; i<_ionmodel->get_ngates(); i++) {
    _gates[_qp][i] = _gates_old[_qp][i];
  }
  
  // Perform one Rush-Larsen time step to propagate forward gating variables
  _ionmodel->rush_larsen_step(_vmem[_qp], _dt, &(_gates[_qp]));

  /**
   * The mono domain equations reads
   *
   * V_t + div( G grad(V)) = I_ion(V)
   *
   * where G is a conductivity tensor, I_ion the current [unit Ampere] generated by the membrane potential V [unit Volt].
   * The ODE for the membrane states S reads
   *
   * S_t = Z(V,S)
   *
   * with V being the membrane potential. 
   * The dependance of I_ion on V and the evolution of the state variables S is given by some membrane model, e.g. Bernus.
   *
   * In propag, the following names are used
   *
   * Vmem = V = membrane potential = variable in reaction-diffusion PDE
   * Iion = I_ion(V) = ion current = reaction term in PDE
   * Imem = diffusion current = div( G grad(V))
   * yyy  = S = cell states = variable in the ODE
   */
  
}
