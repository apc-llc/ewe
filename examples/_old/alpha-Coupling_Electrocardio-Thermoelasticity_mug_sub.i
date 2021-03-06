[Mesh]
 file = mug.e
 
 block_id = '1'
 block_name = 'all'
 
 []

[Variables]
  [./potential] order = FIRST family = LAGRANGE [../]
[]

[AuxVariables]
  [./gate_m]  order = CONSTANT family = MONOMIAL [../]
  [./gate_v]  order = CONSTANT family = MONOMIAL [../]
  [./gate_f]  order = CONSTANT family = MONOMIAL [../]
  [./gate_to] order = CONSTANT family = MONOMIAL [../]
  [./gate_x]  order = CONSTANT family = MONOMIAL [../]
[]

[AuxKernels]
  [./auxgate_m]  type = MaterialStdVectorAux property = 'gates' variable = gate_m  block = all index = 0 [../]
  [./auxgate_v]  type = MaterialStdVectorAux property = 'gates' variable = gate_v  block = all index = 1 [../]
  [./auxgate_f]  type = MaterialStdVectorAux property = 'gates' variable = gate_f  block = all index = 2 [../]
  [./auxgate_to] type = MaterialStdVectorAux property = 'gates' variable = gate_to block = all index = 3 [../]
  [./auxgate_x]  type = MaterialStdVectorAux property = 'gates' variable = gate_x  block = all index = 4 [../]
[]

[Kernels]
  [./diff]
    type = ElectrocardioDiffusion
    variable = potential
    surface_to_volume = 1000.0
    capacitance = 1.0
  [../]
  
  [./ecforcing]
      type = ElectrocardioForcing
      variable = potential
      forcing_function = ElectrocardioForcing_function
      #ion_coeff = 0.0
  [../]

  [./euler]
    type = ElectrocardioTimeDerivative
    variable = potential
  [../]
[]

[BCs]
  [./bc]
    type = NeumannBC
    variable = potential
    boundary = '1 2'
    value = 0
  [../]
[]


[Functions]
  [./pacing]
    type = ParsedFunction
    value = '-40.0*exp(-0.5*pow(x-0.0,2.0)/pow(0.25,2.0))'
  [../]

  [./ElectrocardioForcing_function]
    type = PiecewiseFunction
    default_function = '0'
    functions = 'pacing'
    left      = ' 0.0'
    right     = ' 2.0'
  [../]
[]

[Materials]
  [./cardiac_properties]
    type = CardiacFibresMaterial
    fixed_R = '1 0 0 0 1 0 0 0 1'
    block = all
    outputs = all
  [../]
  
  [./electrocardio]
    type = Electrocardio
    vmem = 'potential'
    block = all
    outputs = all
  [../]
 
  [./conductivity]
    type = ElectrocardioConductivity
    conductivities = '0.006 0.006 0.006'
    block = all
  [../]
[]

[Executioner]
  type = Transient   # Here we use the Transient Executioner
  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
#l_tol = 1e-6
#  l_max_its = 10
  nl_rel_tol = 1e-3
  nl_abs_tol = 1e-8
  nl_rel_step_tol = 1e-8
  nl_max_its = 2

  start_time = 0
  end_time   = 250.0
  dtmin = 0.1
  dtmax = 0.1
  scheme = 'implicit-euler'
[]

[ICs]
  [./ec]
    type = ElectrocardioIC
    variable = potential
    block = all
  [../]
[]
 
[Outputs]
#exodus = true
  [./console]
    type = Console
    perf_log = false
    linear_residuals = false
  [../]
[]
