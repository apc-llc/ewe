#ifndef IIONMODEL
#define IIONMODEL

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
 * model and need not be updated in the time-stepping loop.
 */
class Iionmodel {

public:

  Iionmodel(std::vector<double>* gates, std::vector<double>* gates_dt) : gates(gates), gates_dt(gates_dt){};
  
  // destructor declared virtual to ensure proper polymorphic delete
  virtual ~Iionmodel() {};
  
  //! Computes \\( I_{\rm ion} \\) using the current values of the gating variables and a given membrane potential \\( v \\).
  //! @param[in] v Membrane potential in mV
  //! @param[out] Iion Ion current
  virtual double ionforcing(double v) = 0;
    
  //! Returns the number of ODE-based gating variables of a specific membrane model
  virtual int get_ngates() = 0;

  //! Computes the time-derivative \\( f(v, w) \\) of the gating variables for
  //! the current values of \\( w \\) and a given membrane potential \\( v \\).
  //! New values are stored in #gates_dt.
  //! @param[in] v Membrane potential in mV
  virtual void update_gates_dt(double v) = 0;
  
  //! @todo docu
  //! @param[in]
  //! @param[in]
  virtual void rush_larsen_step(double, double) = 0;
  
  //! Gating variables \\( w \\).
  std::vector<double> * gates;
  
  //! Time derivative \\( f(v,w) \\) of gating variables.
  std::vector<double> * gates_dt;
  
};


#endif // IIONMODEL