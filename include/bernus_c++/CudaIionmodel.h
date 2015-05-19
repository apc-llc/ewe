#ifndef CUDAIIONMODEL
#define CUDAIIONMODEL

#include <vector>
#include <cstdlib>
#include <stdexcept>

/**
 * Defines an interface for membrane models. The mono domain equation for electrocardiology is a reaction
 * diffusion equation
 *
 * \\( C_{m} v_t = \\frac{1}{\\chi} \\nabla \\cdot \\left( G_{\\rm mono} \\nabla v \\right) - I_{\\rm ion}(v, w) \\)
 *
 * where the reaction term \\( I_{\\rm ion} \\) models cell ion currents. Different models exist which 
 * typically involve a set of state or ''gating'' variables \\( w \\) modeled by an ODE
 *
 * \\( w_t = f(v, w) \\).
 *
 * The interface provides two functions: #ionforcing to evaluate \\( I_{\\rm ion} \\) and #update_gates_dt
 * to evaluate \\( f(v, w) \\). It has two member variables: #gates storing \\( w \\) and #gates_dt storing
 * \\( w_t = f(v, w) \\).
 * The ion current from a model can be incorporated as a forcing term into a PDE solver. In addition, explicit
 * integrators for the gating variables can be easily constructed: A forward Euler, e.g.,
 *
 * \\( w_{n+1} = w_{n} + \\Delta t f(v_n, w_n) \\)
 *
 * can be implemented by first calling #update_gates_dt to update #gates_dt and then updating componentwise
 * gates[i] += dt*gates_dt[i]. Note that some models also feature a set of steady-state gating variables which
 * are modeled by algebraic equations. These are not defined or exposed by the interface because they are internal to the
 * model and need not be updated in the time-stepping loop. @todo update
 */
class CudaIionmodel {

public:

  CudaIionmodel() {};
  
  // destructor declared virtual to ensure proper polymorphic delete
  virtual ~CudaIionmodel() {};
  

    //! @todo add docu
  virtual void initialize(double * gates, int n) = 0;
  


  // -Zaikun-
  // add new method for GPU
  // this is simply merge the ionforing and run_larsen into one method
  // to cut GPU communication time

  virtual void rush_larsen_ionforcing(double,\
          double * , \
          double * ,\
          double * ,\
          double *,\
          int ) = 0;
};


#endif // CUDAIIONMODEL