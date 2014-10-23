#include "bernus.hpp"
#include <vector>

// default constructor: initializes all gates to their steady-state value for V = -90.272 mV
bernus::bernus():Iionmodel() {
  gates.resize(bernus::ngates);

  double const Vrest = -90.272;
  
  gates[m_gate]  = bnf.alpha_m(Vrest)/( bnf.alpha_m(Vrest) + bnf.beta_m(Vrest) );
  gates[v_gate]  = bnf.v_inf(Vrest);
  gates[f_gate]  = bnf.alpha_f(Vrest)/( bnf.alpha_f(Vrest) + bnf.beta_f(Vrest) );
  gates[to_gate] = bnf.alpha_to(Vrest)/( bnf.alpha_to(Vrest) + bnf.beta_to(Vrest) );
  gates[x_gate]  = bnf.x_inf(Vrest);
  
  gates_dt.resize(bernus::ngates);
  for (int i=0; i<bernus::ngates; ++i) {
    gates_dt[i] = 0.0;
  }
}

// Initialize gating variables to steady-state values for some other potential then Vrest
bernus::bernus(double V):Iionmodel(){
  gates.resize(bernus::ngates);
  
  gates[m_gate]  = bnf.alpha_m(V)/( bnf.alpha_m(V) + bnf.beta_m(V) );
  gates[v_gate]  = bnf.v_inf(V);
  gates[f_gate]  = bnf.alpha_f(V)/( bnf.alpha_f(V) + bnf.beta_f(V) );
  gates[to_gate] = bnf.alpha_to(V)/( bnf.alpha_to(V) + bnf.beta_to(V) );
  gates[x_gate]  = bnf.x_inf(V);
  
  gates_dt.resize(bernus::ngates);
  for (int i=0; i<bernus::ngates; ++i) {
    gates_dt[i] = 0.0;
  }
  
}

// destructor
bernus::~bernus() {
  // nothing to do
}