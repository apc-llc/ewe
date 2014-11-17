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
 * model and need not be updated in the time-stepping loop. @todo update
 */
class Iionmodel {

public:

  Iionmodel() {};
  
  // destructor declared virtual to ensure proper polymorphic delete
  virtual ~Iionmodel() {};
  
  //! @todo add docu
  virtual void initialize(std::vector<double>* gates) = 0;
  
  //! Computes \\( I_{\rm ion} \\) using the current values of the gating variables and a given membrane potential \\( v \\).
  //! @param[in] v Membrane potential in mV
  //! @param[out] Iion Ion current
  virtual double ionforcing(double v, std::vector<double>* gates) = 0;
    
  //! Returns the number of ODE-based gating variables of a specific membrane model
  virtual int get_ngates() = 0;
  
  //! Computes the time-derivative \\( f(v, w) \\) of the gating variables for
  //! the current values of \\( w \\) and a given membrane potential \\( v \\).
  //! New values are stored in #gates_dt.
  //! @param[in] v Membrane potential in mV
  //! @param[in] gates A vector containing the current values of the gating variables.
  //! @param[out gates_dt A vector containing the temporal derivative of the gating variables.
  virtual void get_gates_dt(double v, std::vector<double>* gates, std::vector<double>* gates_dt) = 0;
  
  //! @todo docu
  //! @param[in] v Membrane potential at beginning of step
  //! @param[in] dt Length of time step
  //! @param[inout] gates Vector with values of gating variables at beginning of time step. It is overwritten with the values after the Rush-Larsen step.
  virtual void rush_larsen_step(double v, double dt, std::vector<double>* gates) = 0;
  
};


#endif // IIONMODEL