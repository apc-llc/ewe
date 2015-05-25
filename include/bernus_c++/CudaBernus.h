#ifndef CUDABERNUS_HPP
#define CUDABERNUS_HPP
#include <algorithm>
#include <vector>
#include <assert.h>
#include "CudaIionmodel.h"


/**
 * Class implementing the Bernus et al. model for ventricular cells:
 *
 * O. Bernus, R. Wilders, C. W. Zemlin, H. Verschelde, A. V. Panfilov:
 * "A computationally efficient electrophysiological model of human ventricular cells";
 * Am. J. Physiol. Heart Circ. Physiol. 282: H2296-H2308, 2002.
 *
 * The model is also described at: https://models.physiomeproject.org/e/5/bernus_wilders_zemlin_verschelde_panfilov_2002.cellml/view
 *
 * The bernus class defined here implements the Iionmodel interface and collects all routines
 * that involve the (time-dependent) gating variables. Static functions, that is functions that
 * do not dependent on the gating variables, are collected in bernus_functions.
 * For a single cell the Bernus model (and many other membrane models) is a differential equation
 *
 * \\( v' = -I_{\rm ion}(v, w) \\\
 *  w' = f(v, w) \\)
 *
 * where \\( v \\) is the membrane potential, \\( w \\) are the state variables of the membrane model
 * (''gating variables'') and both \\( I_{\rm ion} \\) and \\( f \\) are provided by the model.
 * In the Bernus model, \\( I_{\rm ion} \\) is the sum of nine different ion currents:
 * - Sodium current \\( i_{\rm Na} \\)
 * - Calcium current \\( i_{\rm Ca} \\)
 * - Transient outward current \\( i_{\rm to} \\)
 * - Delayed rectifier potassium current \\( i_{\rm K} \\)
 * - Inward rectifier potassium current \\( i_{\textrm{K}, 1} \\)
 * - Calcium background current \\( i_{\rm b, Ca} \\)
 * - Sodium background current \\( i_{\rm b, Na} \\)
 * - Sodium potassium pump \\( i_{\rm Na, K} \\)
 * - Sodium calcium pump \\( i_{\rm Na, Ca} \\)
 *
 * There are five state variables in the Bernus model that are modeled by ODEs:
 * - Sodium current gate \\( m \\)
 * - Sodium current gate \\( v \\)
 * - Calcium current gate \\( f \\)
 * - Transient outward current gate \\( to \\)
 * - Delayed rectifier potassium current gate \\( x \\)
 *
 * <b>A note on physical units</b>: The ionic current return by #ionforcing is in millivolt per millisecond \\( \\textrm{mV}~\\textrm{ms}^{-1} \\): The constants #g_na, #g_ca etc are in
 *
 * \\( \\textrm{nS}~\\textrm{pF}^{-1} = 10^3 \\textrm{S}~\\textrm{F}^{-1} = 10^3~\\textrm{s}^{-1} = \\textrm{ms}^{-1} \\)
 *
 * with S = Siemens and F = Farad, cf. Table 1 in Bernus et al. The membrane potential and equilibrium potential both are in millivolt.
 */
class CudaBernus: public CudaIionmodel {
  
public:
  
  //! Constructor
  CudaBernus();
  
  //! Destructor
  ~CudaBernus();
  
  //! Initialize gating variables to their steady-state values
  //! for the Bernus model resting potential \\( V=-90.272 mV \\)
  // void initialize(std::vector<double>* gates);
  
  // double ionforcing(double,std::vector<double>*);
  
  // int get_ngates();
  
  // void get_gates_dt(double,  std::vector<double>*, std::vector<double>*);
  
  // void rush_larsen_step(double, double,std::vector<double>*);
  void rush_larsen_ionforcing(double , 
          double * , 
          double * ,
          double * ,
          double *,
          int );
           
  void initialize(double *, int);

  //! Static factory function that instantiates a #bernus object and returns a pointer. Called by the #IionmodelFactory class.
  //! @param[out] Iionmodel* A pointer to an object of type #bernus.
  static CudaIionmodel * factory() {
    return new CudaBernus();
  }
  
 };


#endif // CUDABERNUS_HPP