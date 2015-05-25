#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>
#include <cmath>
#include "CudaBernus.h"

#define CUDA_CALL(x) do { cudaError_t err = x; if (( err ) != cudaSuccess ) { \
	printf ("Error \"%s\" at %s :%d \n" , cudaGetErrorString(err), \
		__FILE__ , __LINE__ ) ; exit(-1); \
}} while (0)


#define CUDA_ERR_CHECK(x) \
    do { cudaError_t err = x; if (err != cudaSuccess) { \
        fprintf (stderr, "Error \"%s\" at %s:%d \n", \
        cudaGetErrorString(err), \
        __FILE__, __LINE__); exit(-1); \
    }} while (0);

// using namespace std;
/***********************************************
  the kernel functions 
***********************************************/
  __device__ __const__ int dngates = 5;


  //! Parameter \\( p \\) from Table 4 in Bernus et al. @todo: Make this value celltype-dependent
  __device__ __const__ double p = 1.0;
  
  //! Parameter \\( v_{\\rm shift} \\) from Table 4 in Bernus et al. @todo: Make this value celltype-dependent
  __device__ __const__ double v_shift = 0.0;
  
  //! Intracellular calcium concentration \\( [\\textrm{Ca}^{2+}]_i \\) from Table 1 in Bernus et al.
  __device__ __const__ double ca_i = 0.0004;
  
  //! Extracellular calcium concentration \\( [\\textrm{Ca}^{2+}]_e \\) from Table 1 in Bernus et al.
  __device__ __const__ double ca_e = 2.0;
  
  //! Intracellular sodium concentration \\( [\\textrm{Na}^{+}]_i \\) from Table 1 in Bernus et al
  __device__ __const__ double na_i = 10.0;
  
  //! Extracellular sodium concentration \\( [\\textrm{Na}^{+}]_e \\) from Table 1 in Bernus et al
  __device__ __const__ double na_e = 138.0;
  
  //! Intracellular potassium concentration \\( [\\textrm{K}^{+}]_i \\) from Table 1 in Bernus et al
  __device__ __const__ double k_i  = 140.0;

  //! Extracellular potassium concentration \\( [\\textrm{K}^{+}]_e \\) from Table 1 in Bernus et al
  __device__ __const__ double k_e  = 4.0;
  
  //! Universal gas constant in \\( \\textrm{J}~\\textrm{mol}^{-1}~\\textrm{K}^{-1} \\); note that \\( \\textrm{J} = \\textrm{C}~\\textrm{V} \\).
  //! Because membrane potential in the Bernus model is expressed in milli-Volt, rescale #R to also be in \\( \\textrm{mV} \\).
  __device__ __const__ double R = 8.3144621*1e3;
  
  //! Absolute temperature (in Kelvin, for value in Celsius see Table 1 in Bernus et al.)
  __device__ __const__ double T = 37.0 + 273.15;
  
  //! Faraday constant in \\( \\textrm{C}~\\textrm{mol}^{-1} \\).
  __device__ __const__ double Fa = 9.64853399*1e4;
  
  //! Equilibrium potential \\( E_{\\textrm Na} \\) in millivolt, pp. H2306 in Bernus et al. @todo Would be nicer with constexpr, but use of log in constexpr is not yet supported by clang++
  __device__  double e_na = 70.148612720557793;
  //(R*T/Fa)*log(na_e/na_i);
  
  //! Equilibrium potential \\( E_{\\textrm Ca} \\) in millivolt, pp. H2306 in Bernus et al. @todo Would be nicer with constexpr, but use of log in constexpr is not yet supported by clang+
  __device__  double e_ca = 1.138180432445409e+02;
  //(R*T/(2.0*Fa))*log(ca_e/ca_i) ;

  //! Equilibrium potential \\( E_{to} \\) in millivolt, pp. H2306 in Bernus et al. @todo Would be nicer with constexpr, but use of log in constexpr is not yet supported by clang++
  __device__  double e_to =  -70.792119356993638;
  //(R*T/Fa)*log( (0.043*na_e + k_e)/(0.043*na_i + k_i) );
  
  //! Equilibrium potential \\( E_{\\textrm K} \\) in millivolt, pp. H2306 in Bernus et al. @todo Would be nicer with constexpr, but use of log in constexpr is not yet supported by clang++
  __device__ double e_k = -95.022562085317546;
  //(R*T/Fa)*log(k_e/k_i);
  
  
  /*
   * Implementation of class functions; kept in header for easier inlining.
   * See Bernus et al. 2002 or https://models.cellml.org/e/5/bernus_wilders_zemlin_verschelde_panfilov_2002.cellml/@@cellml_math
   * for the formulas.
   */

  /*
   * (1) Sodium current i_Na (4 Functions)
   */

// m-gate


static __device__ __inline__ double alpha_m(double V)
{
 return 0.32*(V+47.13)/(1.0 - exp(-0.1*(V+47.13)));
}

static __device__ __inline__ double beta_m(double V)
{ 
 return 0.08*exp(-V/11.0); 
}
// v-gate
static __device__ __inline__ double v_inf(double V)
{ 
 return 0.5*(1.0 - (tanh(7.74 + 0.12*V))); 
}

static __device__ __inline__ double tau_v(double V)
{ 
 return 0.25 + 2.24*( 1.0-(tanh(7.74 + 0.12*V)) )/( 1.0 - tanh(0.07*(92.4+V)) ); 
}

 

 /*
  * (2) Calcium current i_Ca (5 functions)
  */

 // d-gate

static __device__ __inline__ double alpha_d(double V)
{ 
 return 14.98*exp(-0.5*pow( (V-22.36)/16.68, 2.0 ))/(16.68*sqrt(2.0*M_PI)); 
}

static __device__ __inline__ double beta_d(double V)
{ 
 return 0.1471 - 5.3*exp(-0.5*pow( (V-6.27)/14.93, 2.0))/(14.93*sqrt(2.0*M_PI)) ; 
}

static __device__ __inline__ double d_inf(double V)
{ 
 return alpha_d(V)/(alpha_d(V)+beta_d(V)); 
}

// f-gate

static __device__ __inline__ double alpha_f(double V)
{ 
 return 6.87e-3/(1.0 + exp( -(6.1546-V)/6.12) ); 
}

static __device__ __inline__ double beta_f(double V)
{ 
 return (0.069*exp(-0.11*(V+9.825))+0.011)/(1.0 + exp(-0.278*(V+9.825))) + 5.75e-4; 
}

// f_Ca-gate
static __device__ __inline__ double f_ca(double V)
{ 
 return 1.0/(1.0 + ca_i/0.0006); 
}

/*
* (3) Transient outward current i_to (7 functions)
*/

// r-gate

static __device__ __inline__ double  alpha_r(double V)
{ 
 return 0.5266*exp(-0.0166*(V-42.2912))/(1.0 + exp(-0.0943*(V-42.2912))); 
}

static __device__ __inline__ double  beta_r(double V)
{
  return (5.186e-5*V+0.5149*exp(-0.1344*(V-5.0027)))/(1.0 + exp(-0.1348*(V-5.186e-5))); 
}

static __device__ __inline__ double  r_inf(double V)
{ 
 return alpha_r(V)/(alpha_r(V)+beta_r(V)); 
}

// to-gate
static __device__ __inline__ double  alpha_to(double V)
{ 
  return (5.612e-5*V+0.0721*exp(-0.173*(V+34.2531)))/(1.0 + exp(-0.173*(V+34.2531))); 
}

static __device__ __inline__ double  beta_to(double V)
{ 
  return (1.215e-4*V + 0.0767*exp(-1.66e-9*(V+34.0235)))/(1.0 + exp(-0.1604*(V+34.0235))); 
}

static __device__ __inline__ double  tau_to(double V)
{ 
  return 1.0/( p*alpha_to(V) + p*beta_to(V)); 
}

static __device__ __inline__ double  to_inf(double V)
{ return alpha_to(V - v_shift)/( alpha_to(V- v_shift) + beta_to(V - v_shift)); }

/*
* (4) Delayed rectifier potassium current i_K (3 functions)
*/

// X-gate
static __device__ __inline__ double  x_inf(double V)
{ 
  return 0.988/(1.0 + exp(-0.861-0.062*V)); 
}

static __device__ __inline__ double  tau_x_a(double V)
{
 return 40.0*(1.0 - tanh(160.0 + 2.0*V)); 
}

static __device__ __inline__ double  tau_x(double V)
{
  return 240.0*exp(-pow( 25.5+V, 2.0)/156.0) + 182.0*(1.0 + tanh(0.154 + 0.0116*V)) + tau_x_a(V); 
}

 
 /*
  * (5) Inward rectifier potassium current i_K1 (3 functions)
  */

 // K1-gate

static __device__ __inline__ double  alpha_k1(double V)
{ 
  return 0.1/(1.0 + exp(0.06*(V-e_k - 200.0))); 
}

static __device__ __inline__ double  beta_k1(double V)
{
  //NOTE: The e_k1 in Bernus et al. is a typo and should be e_k; cf. cellml.org
  return (3.0*exp(2e-4*(V-e_k+100.0)) + exp(0.1*(V-e_k-10.0)))/( 1.0 + exp(-0.5*(V - e_k)) ); 
}

static __device__ __inline__ double  k1_inf(double V)
{ 
  return alpha_k1(V)/(alpha_k1(V) + beta_k1(V)); 
}

  
/*
* (8) Sodium potassium pump (3 functions)
*/
static __device__ __inline__ double  f_nak(double V)
{
  double sigma = 0.1428*( exp(na_e/67.3) - 1.0 );
  return 1.0/(1.0 + 0.1245*exp(-0.0037*V) + 0.0365*sigma*exp(-0.0037*V)); 
}

static __device__ __inline__ double  f_nak_a(double V)
{ 
  return (1.0/(1.0 + pow( 10.0/na_i, 1.5 )))*( k_e/(k_e+1.5) ); 
}

  /*
   * (9) Sodium calcium pump i_NaCa (1 function)
   */
static __device__ __inline__ double f_naca(double V)
{
  double a = 1.0/( (pow(87.5, 3.0) + pow(na_e, 3.0)) * (1.38+ca_e) * (1.0 + 0.1*exp(-0.024*V)) );
  return a*( pow(na_i, 3.0) * ca_e * exp(0.013*V) - pow( na_e, 3.0)*ca_i*exp(-0.024*V) ); 
} 

//! Index of gating variable \\( m \\) in #gates
__device__ __const__ int m_gate  = 0;
  
  //! Index of gating variable \\( v \\) in #gates
__device__ __const__ int v_gate  = 1;
  
  //! Index of gating variable \\( f \\) in #gates
__device__ __const__ int f_gate  = 2;
  
  //! Index of gating variable \\( to \\) in #gates
__device__ __const__ int to_gate = 3;
  
  //! Index of gating variable \\( x \\) in #gates
__device__ __const__ int x_gate  = 4;
  


 //! Constant \\( g_{\rm Na} \\) from Table 1 in Bernus et al.
__device__ __const__ double g_na   = 16.0;

  //! Constant \\( g_{\rm Ca} \\) from Table 1 in Bernus et al.
__device__ __const__ double g_ca   = 0.064;
  
  //! Constant \\( g_{\rm to} \\) from Table 1 in Bernus et al.
__device__ __const__ double g_to   = 0.4;
  
  //! Constant \\( g_{\rm K} \\) from Table 1 in Bernus et al.
__device__ __const__ double g_k    = 0.019;
  
  //! Constant \\( g_{\textrm{K},1} \\) from Table 1 in Bernus et al.
__device__ __const__ double g_k1   = 3.9;
  
  //! Constant \\( g_{\rm Na,b} \\) from Table 1 in Bernus et al.
__device__ __const__ double g_na_b = 0.001;
  
  //! Constant \\( g_{\rm Ca,b} \\) from Table 1 in Bernus et al.
__device__ __const__ double g_ca_b = 0.00085;
  
  //! Constant \\( g_{\rm Na,K} \\) from Table 1 in Bernus et al.
__device__ __const__ double g_nak  = 1.3;
  
  //! Constant \\( g_{\rm Na,Ca} \\) from Table 1 in Bernus et al.
__device__ __const__ double g_naca = 1000.0;

// Sodium current i_Na
static __device__ __inline__ double i_na(double V, double * gates)
{
  return g_na*pow(gates[m_gate], 3.0)*pow(gates[v_gate], 2.0)*(V - e_na);
}  

// Calcium current i_Ca
static __device__ __inline__ double i_ca(double V,double * gates)
{
  return g_ca*(d_inf(V))*gates[f_gate]*(f_ca(V))*(V-e_ca);
}

// Transient outward current i_to
static __device__ __inline__ double i_to(double V,double * gates)
{  
  return g_to*(r_inf(V))*gates[to_gate]*(V-e_to);
}

// Delated rectifier potassium current i_K
static __device__ __inline__ double i_k(double V, double * gates)
{
  return g_k*pow( gates[x_gate], 2.0)*(V-e_k);
}

// Inward rectifier potassium current i_K1
static __device__ __inline__ double i_k1(double V)
{
  return g_k1*(k1_inf(V))*(V-e_k);
}

// Calcium background current
static __device__ __inline__ double i_b_ca(double V)
{
  return g_ca_b*(V-e_ca);
}

// Sodium background current
static __device__ __inline__ double i_b_na(double V)
{
  return g_na_b*(V - e_na);
}

// Sodium potassium pump
static __device__ __inline__ double i_na_k(double V)
{
  return g_nak*(f_nak(V))*(f_nak_a(V));
}

// Sodium calcium pump
static __device__ __inline__ double i_na_ca(double V)
{
  return g_naca*(f_naca(V));
}


/*
 * To faciliate inlining, the ion current functions are implemented here, in the header file.
 * The keyword 'inline' allow e.g. the use of the -Winline flag for gcc to issue a warning if
 * the compiler was unable to actually inline the respective function.
 */
/*

  gates :
    0 -4 are five gate
    5 : V (not anymore)
    6 : Iion (not anymore)
*/


__global__ void rush_larsen_ionforce( double dt, double * vmem, double * oldgates, double * gates, double * Iion, double qpsize) 
{
  int idx = blockDim.x * blockIdx.x + threadIdx.x;
  if(idx < qpsize){
      // get the voltage for the current thread
      double V = vmem[idx];

      // the ionforcing part here 
      double * tmp = (oldgates + idx * dngates);
      Iion[idx] = i_na(V, tmp )+i_ca(V, tmp)+i_to(V, tmp)+i_k(V, tmp)+i_k1(V)+i_b_ca(V)+i_b_na(V)+i_na_k(V)+i_na_ca(V);

      // copy the oldgates back into gates
      for (int i=0; i< dngates; i++) {
        gates[idx * dngates +  i] = oldgates[idx * dngates + i];
      }

      // the real rush_larsen_step
      double y_inf;
      double tau_y;
      
      // m-gate
      y_inf = alpha_m(V)/( alpha_m(V) + beta_m(V) );
      tau_y = 1.0/( alpha_m(V) + beta_m(V) );
      gates[idx * dngates + m_gate] *= exp(-dt/tau_y);
      gates[idx * dngates + m_gate] += (1.0 - exp(-dt/tau_y))*y_inf;
      
      // // f-gate
      y_inf = alpha_f(V)/( alpha_f(V) + beta_f(V) );
      tau_y = 1.0/( alpha_f(V) + beta_f(V) );
      gates[idx * dngates + f_gate] *= exp(-dt/tau_y);
      gates[idx * dngates + f_gate] += (1.0 - exp(-dt/tau_y))*y_inf;
      
      // // to-gate
      y_inf = alpha_to(V)/( alpha_to(V) + beta_to(V) );
      tau_y = 1.0/( alpha_to(V) + beta_to(V) );
      gates[idx * dngates + to_gate] *= exp(-dt/tau_y);
      gates[idx * dngates + to_gate] += (1.0 - exp(-dt/tau_y))*y_inf;
      
      // // v-gate
      y_inf = v_inf(V);
      tau_y = tau_v(V);
      gates[idx * dngates + v_gate] *= exp(-dt/tau_y);
      gates[idx * dngates + v_gate] += (1.0 - exp(-dt/tau_y))*y_inf;
      
      // // x-gate
      y_inf = x_inf(V);
      tau_y = tau_x(V);
      gates[idx * dngates + x_gate] *= exp(-dt/tau_y);
      gates[idx * dngates + x_gate] += (1.0 - exp(-dt/tau_y))*y_inf;
  }
  
}

/* 
  input : 
     gates is a pointer to pointer
     qpsize is the # of all quadrature points
*/

__global__ void init(double * gates, double qpsize) {
  // Resting potential of Bernus model
  double Vrest = -90.272;
  int idx = blockDim.x * blockIdx.x + threadIdx.x;
  if(idx < qpsize){
    gates[idx * dngates + m_gate]  = alpha_m(Vrest)/(alpha_m(Vrest) + beta_m(Vrest) );
    gates[idx * dngates + v_gate]  = v_inf(Vrest);
    gates[idx * dngates + f_gate]  = alpha_f(Vrest)/( alpha_f(Vrest) + beta_f(Vrest) );
    gates[idx * dngates + to_gate] = alpha_to(Vrest)/( alpha_to(Vrest) + beta_to(Vrest) );
    gates[idx * dngates + x_gate]  = x_inf(Vrest);  
  }
}


/***********************************************
  end of the kernel functions 
***********************************************/

CudaBernus::CudaBernus()
{ 
  
}

void CudaBernus::initialize(double * _gates, int qpsize)
{ 
  int ngates = 5;
  double * d_gates;

  //h_gates = (double *)malloc( (ngates) * qpsize  * sizeof(double));
  CUDA_ERR_CHECK(cudaMalloc((void **)&d_gates,  (ngates) *qpsize* sizeof(double)) );

  int nx =  64;
  int ny =  64;
  int nxb = qpsize/nx/ny;
  if ((qpsize - nxb * nx) > 0)
    nxb++;
  dim3 blocks(nxb, 1 ,1);     // cuda 1.x has 1D and 2D grids, cuda 2.x adds 3D grids
  dim3 threadsPerBlock(nx, ny,1 );  // cuda 1.x has 1D, 2D, and 3D blocks 
  init<<< blocks, threadsPerBlock>>>(d_gates, qpsize);
  CUDA_ERR_CHECK(cudaGetLastError());
  //CUDA_ERR_CHECK(cudaDeviceSynchronize());
  CUDA_ERR_CHECK(cudaMemcpy(_gates, d_gates, (ngates) * sizeof(double), cudaMemcpyDeviceToHost));

  // copy the value back to _gates
  // for(int i = 0; i < qpsize; i++){
  //   for(int j = 0 ;j < ngates;j++){
  //     (*_gates)[i][j] = h_gates[i * ngates + j] ;
  //   }
  // }
}


void CudaBernus::rush_larsen_ionforcing(double _dt, 
          double * _vmem, 
          double * _gates_old,
          double * _gates,
          double * _Iion,
          int qpsize) 
{
  
  // double * h_gates, *h_oldgates, *h_vmem, *h_Iion;
  // h_gates = (double *)malloc( (dngates) * qpsize  * sizeof(double));
  // h_oldgates = (double *)malloc( (dngates) * qpsize  * sizeof(double));
  // h_vmem = (double *)malloc( qpsize  * sizeof(double));
  // h_Iion = (double *)malloc( qpsize  * sizeof(double));
  
  // h_gates = &_gates_old[0];
  // h_oldgates = &_gates_old[0];
  // for(int i = 0; i < qpsize; i++){
  //   h_vmem[i] = (*_vmem)[i];
  //   for(int j = 0 ;j < dngates;j++){
  //     h_gates[i * qpsize + j] = _gates[i][j];
  //     h_oldgates[i * qpsize + j] = _gates_old[i][j];
  //   }
  //}

  /*
    -----------------------------------------------------
  */

  clock_t timer = clock();

  /*******************************************
   allocate memory on gpu and copy data to gpu
  *********************************************/

  double * d_gates , * d_oldgates, * d_vmem, * d_Iion;

  CUDA_ERR_CHECK(cudaMalloc((void **)&d_gates,  (dngates) *qpsize* sizeof(double)) );
  CUDA_ERR_CHECK(cudaMalloc((void **)&d_oldgates,  (dngates) *qpsize* sizeof(double)) );
  CUDA_ERR_CHECK(cudaMalloc((void **)&d_Iion, qpsize* sizeof(double)) );
  CUDA_ERR_CHECK(cudaMalloc((void **)&d_vmem, qpsize* sizeof(double)) );

  CUDA_ERR_CHECK(cudaMemcpy(d_gates, _gates,dngates * qpsize * sizeof(double), cudaMemcpyHostToDevice));
  CUDA_ERR_CHECK(cudaMemcpy(d_oldgates, _gates_old, dngates * qpsize * sizeof(double), cudaMemcpyHostToDevice));
  CUDA_ERR_CHECK(cudaMemcpy(d_vmem, _vmem,  qpsize * sizeof(double), cudaMemcpyHostToDevice));
  
  /*******************************************
    set up the kernel
  *********************************************/

  int nx =  64;
  int ny =  64;
  int nxb = qpsize/nx/ny;
  if ((qpsize - nxb * nx) > 0)
    nxb++;
  dim3 blocks(nxb, 1 ,1);     // cuda 1.x has 1D and 2D grids, cuda 2.x adds 3D grids
  dim3 threadsPerBlock(nx, ny,1 );  // cuda 1.x has 1D, 2D, and 3D blocks 

  // copy the two gates array to device
  CUDA_ERR_CHECK(cudaMemcpy(d_gates, _gates, (dngates) * qpsize * sizeof(double), cudaMemcpyHostToDevice));
  CUDA_ERR_CHECK(cudaMemcpy(d_oldgates, _gates_old, (dngates) * qpsize * sizeof(double), cudaMemcpyHostToDevice));

  rush_larsen_ionforce<<< blocks, threadsPerBlock>>>( _dt,  d_vmem, d_oldgates, d_gates, d_Iion, qpsize);
  
  // before copy back, need to synchronize

  CUDA_ERR_CHECK(cudaDeviceSynchronize());
  CUDA_ERR_CHECK(cudaGetLastError());       
  /*******************************************
    copy data back to Host and time the 
  *********************************************/
  CUDA_ERR_CHECK(cudaMemcpy(_gates, d_gates, (dngates) * qpsize * sizeof(double), cudaMemcpyDeviceToHost));
  CUDA_ERR_CHECK(cudaMemcpy(_gates_old, d_oldgates, (dngates) * qpsize * sizeof(double), cudaMemcpyDeviceToHost));
  CUDA_ERR_CHECK(cudaMemcpy(_Iion ,d_Iion, qpsize * sizeof(double), cudaMemcpyDeviceToHost));
  CUDA_ERR_CHECK(cudaMemcpy(_vmem ,d_vmem, qpsize * sizeof(double), cudaMemcpyDeviceToHost));
  
 
  timer = clock() - timer;
  double time_in_sec = ( (double) timer )/CLOCKS_PER_SEC;
  std::cout << "Total runtime:                       " << time_in_sec << std::endl;


  /****************************************************
    copy data from Host to the corresponding variables
  *****************************************************/

  // for(int i = 0; i < qpsize; i++){
  //   for(int j = 0 ;j < ngates;j++){
  //       (*_gates)[i][j] = h_gates[i * ngates + j] ;
  //       (*_gates)[i][j] = h_gates[i * ngates + j] ;
  //   }
  //   (*_vmem)[i] = h_vmem[i];
  //   (*_Iion)[i] = h_Iion[i];
  // }

  /****************************************************
        Free points both on Host and Deivce
  *****************************************************/

  cudaFree(d_gates);
  cudaFree(d_oldgates);
  cudaFree(d_vmem);
  cudaFree(d_Iion);
  // free(h_gates);
  // free(h_oldgates);
  // free(h_vmem);
  // free(h_Iion);


}


