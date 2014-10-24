#ifndef BERNUS_FUNCTIONS_HPP
#define BERNUS_FUNCTIONS_HPP

#include <cmath>

/**
 * Header file that provides a collection of functions for the Bernus model. Note that #bernus_functions has only a header which contains only static variables and functions which do not depend on the gating variables.
 * Computation of the actual ion currents, which do depend on the gating variables, is done by the #bernus class.
 *
 * The constants and formulas below are all from
 *
 * O. Bernus, R. Wilders, C. W. Zemlin, H. Verschelde, A. V. Panfilov:
 * "A computationally efficient electrophysiological model of human ventricular cells";
 * Am. J. Physiol. Heart Circ. Physiol. 282: H2296-H2308, 2002
 *
 * and
 *
 * https://models.physiomeproject.org/e/5/bernus_wilders_zemlin_verschelde_panfilov_2002.cellml/view
 */
class bernus_functions
{
  
  public:
  
  bernus_functions(){};
  
  ~bernus_functions(){};
  
  // TODO: To later be able to easily run the whole ion channel
  // model in single precision, introduce a ionprec typedef accuracy
  
  // Sodium current parameter
  
  //! Computes the sodium current parameter \\( \\alpha_m \\) , eq. (14) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] alpha_m Parameter
  static double alpha_m(double);

  //! Computes the sodium current parameter \\( \\beta_m \\), eq. (15) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] beta_m Parameter
  static double beta_m(double);
  
  //! Computes the sodium current parameter \\( v_{\\infty} \\), eq. (16) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] v_inf Parameter
  static double v_inf(double);
  
  //! Computes the sodium current parameter \\( \\tau_v \\), eq. (17) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] tau_v Parameter
  static double tau_v(double);
  
  // Calcium current parameter
  
  //! Computes the calcium current parameter \\( d_{\\infty} \\), eq. (19) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] d_inf Parameter
  static double d_inf(double);
  
  //! Computes the calcium current parameter \\( \\alpha_d \\), eq. (20) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] alpha_d Parameter
  static double alpha_d(double);
  
  //! Computes the calcium current parameter \\( \\beta_d \\), eq. (21) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] beta_d Parameter
  static double beta_d(double);
  
  //! Computes the calcium current parameter \\( \\alpha_f \\), eq. (22) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] alpha_f Parameter
  static double alpha_f(double);
  
  //! Computes the calcium current parameter \\( \\beta_f \\), eq. (23) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] beta_f Parameter
  static double beta_f(double);
  
  //! Computes the calcium current parameter \\( f_{Ca} \\), eq. (24) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] f_ca Parameter
  static double f_ca(double);
  
  // Transient outward current parameter
  
  //! Computes the transient outward current parameter \\( r_{\\infty} \\), eq. (26) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] r_inf Parameter
  static double r_inf(double);
  
  //! Computes the transient outward current parameter \\( \\alpha_r \\), eq. (27) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] alpha_r Parameter
  static double alpha_r(double);
  
  //! Computes the transient outward current parameter \\( \\beta_r \\), eq. (28) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] beta_r Parameter
  static double beta_r(double);
  
  //! Computes the transient outward current parameter \\( \\alpha_{to} \\), eq. (29) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] alpha_to Parameter
  static double alpha_to(double);
  
  //! Computes the transient outward current parameter \\( \\beta_{to} \\), eq. (30) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] beta_to Parameter
  static double beta_to(double);
  
  //! Computes the transient outward current parameter \\( \\tau_{to}\\), eq. (31) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] tau_to Parameter
  static double tau_to(double);
  
  //! Computes the transient outward current parameter \\( \\tau_{\\infty}\\), eq. (32) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] tau_inf Parameter
  static double to_inf(double);
  
  // Delayed rectifier potassium current
  
  //! Computes the delayed rectifier potassium current parameter \\( x_{\\infty}\\), eq. (34) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] x_inf Parameter
  //! @todo: Modify so that this can be varied for different cell types.
  static double x_inf(double);
  
  //! Computes the delayed rectifier potassium current parameter \\( \\tau_{x}\\), eq. (35) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] tau_x Parameter
  static double tau_x(double);
  
  //! Computes the delayed rectifier potassium current parameter \\( \\tau_{x, a}\\), eq. (36) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] tau_x_a Parameter
  //! @todo: Modify so that this can be varied for different cell types.
  static double tau_x_a(double);
  
  // Inward rectifier potassium current
  
  //! Computes the inward rectifier potassium current parameter \\( k1_{\\infty}\\), eq. (40) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] k1_inf Parameter
  static double k1_inf(double);

  //! Computes the inward rectifier potassium current parameter \\( \\alpha_{k1}\\), eq. (41) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] alpha_k1 Parameter
  static double alpha_k1(double);
  
  //! Computes the inward rectifier potassium current parameter \\( \\beta_{k1}\\), eq. (41) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] beta_k1 Parameter
  static double beta_k1(double);
  
  // Calcium background current: No parameter functions needed
  
  // Sodium background current: No parameter functions needed
  
  // Sodium potassium pump
  
  //! Computes the sodium potassium pump parameter \\( f_{Na, K}\\), eq. (46) in Bernus et al.
  //! Note that the parameter \\( \\sigma \\), eq. (48) in Bernus et al., is also computed in this function.
  //! @param[in] V Membrane potential
  //! @param[out] f_nak Parameter
  static double f_nak(double);
  
  //! Computes the sodium potassium pump parameter \\( f_{Na, K}' \\), eq. (47) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] f_nak_a Parameter
  static double f_nak_a(double);
  
  // Sodium calcium pump
  
  //! Computes the sodium calcium pump parameter \\( f_{Na, Ca} \\), eq. (50) in Bernus et al.
  //! @param[in] V Membrane potential
  //! @param[out] f_naca Parameter
  static double f_naca(double);
  
  //! Parameter \\( p \\) from Table 4 in Bernus et al. @todo: Make this value celltype-dependent
  double static constexpr p = 1.0;
  
  //! Parameter \\( v_{\\rm shift} \\) from Table 4 in Bernus et al. @todo: Make this value celltype-dependent
  double static constexpr v_shift = 0.0;
  
  //! Intracellular calcium concentration \\( [\\textrm{Ca}^{2+}]_i \\) from Table 1 in Bernus et al.
  double static constexpr ca_i = 0.0004;
  
  //! Extracellular calcium concentration \\( [\\textrm{Ca}^{2+}]_e \\) from Table 1 in Bernus et al.
  double static constexpr ca_e = 2.0;
  
  //! Intracellular sodium concentration \\( [\\textrm{Na}^{+}]_i \\) from Table 1 in Bernus et al
  double static constexpr na_i = 10.0;
  
  //! Extracellular sodium concentration \\( [\\textrm{Na}^{+}]_e \\) from Table 1 in Bernus et al
  double static constexpr na_e = 138.0;
  
  //! Intracellular potassium concentration \\( [\\textrm{K}^{+}]_i \\) from Table 1 in Bernus et al
  double static constexpr k_i  = 140.0;

  //! Extracellular potassium concentration \\( [\\textrm{K}^{+}]_e \\) from Table 1 in Bernus et al
  double static constexpr k_e  = 4.0;
  
  //! Universal gas constant
  double static constexpr R = 8.3144621;
  
  //! Absolute temperature (in Kelvin, for value in Celsius see Table 1 in Bernus et al.)
  double static constexpr T = 37.0 + 273.15;
  
  //! Faraday constant (scaled).
  /**  Note: In SI units it is \\( \\textrm{Fa}=9.6485 \\times 10^4~\\textrm{C}~\\textrm{mol}^{-1} \\), but parameters for Bernus model are in \\( \\textrm{mM} \\), i.e. \\( 10^{-3} \\times \\textrm{mol} \\), therefore use scaled value.*/
  double static constexpr Fa = 96.485;
  
  //! Equilibrium potential \\( E_{\\textrm Na} \\), pp. H2306 in Bernus et al.
  double static constexpr e_na = (R*T/Fa)*log(na_e/na_i);

  //! Equilibrium potential \\( E_{\\textrm Ca} \\), pp. H2306 in Bernus et al.
  double static constexpr e_ca = R*T/(2.0*Fa)*log(ca_e/ca_i);

  //! Equilibrium potential \\( E_{to} \\), pp. H2306 in Bernus et al.
  double static constexpr e_to = (R*T/Fa)*log( (0.043*na_e + k_e)/(0.043*na_i + k_i) );
  
  //! Equilibrium potential \\( E_{\\textrm K} \\), pp. H2306 in Bernus et al.
  double static constexpr e_k  = (R*T/Fa)*log(k_e/k_i);
  
};

/*
 * Implementation of class functions; kept in header for easier inlining.
 * See Bernus et al. 2002 or https://models.cellml.org/e/5/bernus_wilders_zemlin_verschelde_panfilov_2002.cellml/@@cellml_math
 * for the formulas.
 */

/*
 * (1) Sodium current i_Na (4 Functions)
 */

// m-gate
inline double bernus_functions::alpha_m(double V)
{ return 0.32*(V+47.13)/(1.0 - exp(-0.1*(V+47.13))); }

inline double bernus_functions::beta_m(double V)
{ return 0.08*exp(-V/11.0); }

// v-gate
inline double bernus_functions::v_inf(double V)
{ return 0.5*(1.0 - (tanh(7.74 + 0.12*V))); }

inline double bernus_functions::tau_v(double V)
{ return 0.25 + 2.24*( 1.0-(tanh(7.74 + 0.12*V)) )/( 1.0 - tanh(0.07*(92.4+V)) ); }

/*
 * (2) Calcium current i_Ca (5 functions)
 */

// d-gate
inline double bernus_functions::d_inf(double V)
{ return alpha_d(V)/(alpha_d(V)+beta_d(V)); }

inline double bernus_functions::alpha_d(double V)
{ return 14.98*exp(-0.5*pow( (V-22.36)/16.68, 2.0 ))/(16.68*sqrt(2.0*M_PI)); }

inline double bernus_functions::beta_d(double V)
{ return 0.1471 - 5.3*exp(-0.5*pow( (V-6.27)/14.93, 2.0 ))/(14.93*sqrt(2.0*M_PI)) ; }

// f-gate
inline double bernus_functions::alpha_f(double V)
{ return 6.87e-3/(1.0 + exp( -(6.1546-V)/6.12) ); }

inline double bernus_functions::beta_f(double V)
{ return (0.069*exp(-0.11*(V+9.825))+0.011)/(1.0 + exp(-0.278*(V+9.825))) + 5.75e-4; }

// f_Ca-gate
inline double bernus_functions::f_ca(double V)
{ return 1.0/(1.0 + bernus_functions::ca_i/0.0006); }

/*
 * (3) Transient outward current i_to (7 functions)
 */

// r-gate
inline double bernus_functions::r_inf(double V)
{ return alpha_r(V)/(alpha_r(V)+beta_r(V)); }

inline double bernus_functions::alpha_r(double V)
{ return 0.5266*exp(-0.0166*(V-42.2912))/(1.0 + exp(-0.0943*(V-42.2912))); }

inline double bernus_functions::beta_r(double V)
{ return (5.186e-5*V+0.5149*exp(-0.1344*(V-5.0027)))/(1.0 + exp(-0.1348*(V-5.186e-5))); }

// to-gate
inline double bernus_functions::alpha_to(double V)
{ return (5.612e-5*V+0.0721*exp(-0.173*(V+34.2531)))/(1.0 + exp(-0.173*(V+34.2531))); }

inline double bernus_functions::beta_to(double V)
{ return (1.215e-4*V + 0.0767*exp(-1.66e-9*(V+34.0235)))/(1.0 + exp(-0.1604*(V+34.0235))); }

inline double bernus_functions::tau_to(double V)
{ return 1.0/( bernus_functions::p*alpha_to(V) + bernus_functions::p*beta_to(V)); }

inline double bernus_functions::to_inf(double V)
{ return alpha_to(V - bernus_functions::v_shift)/( alpha_to(V-bernus_functions::v_shift) + beta_to(V - bernus_functions::v_shift)); }

/*
 * (4) Delayed rectifier potassium current i_K (3 functions)
 */

// X-gate
inline double bernus_functions::x_inf(double V)
{ return 0.988/(1.0 + exp(-0.861-0.062*V)); }

inline double bernus_functions::tau_x(double V)
{ return 240.0*exp(-pow( 25.5+V, 2)/156.0) + 182.0*(1.0 + tanh(0.154 + 0.0116*V)) + tau_x_a(V); }

inline double bernus_functions::tau_x_a(double V)
{ return 40.0*(1.0 - tanh(160.0 + 2.0*V)); }

/*
 * (5) Inward rectifier potassium current i_K1 (3 functions)
 */

// K1-gate
inline double bernus_functions::k1_inf(double V)
{ return alpha_k1(V)/(alpha_k1(V) + beta_k1(V)); }

inline double bernus_functions::alpha_k1(double V)
{ return 0.1/(1.0 + exp(0.06*(V-e_k - 200.0))); }

inline double bernus_functions::beta_k1(double V)
{
//NOTE: The e_k1 in Bernus et al. is a typo and should be e_k; cf. cellml.org
return (3.0*exp(2e-4*(V-e_k+100.0)) + exp(0.1*(V-e_k-10.0)))/( 1.0 + exp(-0.5*(V - e_k)) ); }

/*
 * (8) Sodium potassium pump (3 functions)
 */
inline double bernus_functions::f_nak(double V)
{
double sigma = 0.1428*( exp(na_e/67.3) - 1.0 );
return 1.0/(1.0 + 0.1245*exp(-0.0037*V) + 0.0365*sigma*exp(-0.0037*V)); }

inline double bernus_functions::f_nak_a(double V)
{ return (1.0/(1.0 + pow( 10.0/na_i, 1.5 )))*( k_e/(k_e+1.5) ); }

/*
 * (9) Sodium calcium pump i_NaCa (1 function)
 */
inline double bernus_functions::f_naca(double V)
{
  double a = 1.0/( (pow(87.5, 3.0) + pow(na_e, 3.0)) * (1.38+ca_e) * (1.0 + 0.1*exp(-0.024*V)) );
  return a*( pow(na_i, 3.0) * ca_e * exp(0.013*V) - pow( na_e, 3.0)*ca_i*exp(-0.024*V) ); } //TODO: Insert correct function

#endif // BERNUS_FUNCTIONS_HPP