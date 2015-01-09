[Mesh]
  file = 07-heart_geometry_new.e
  block_id = '1'
  block_name = 'all'
  displacements = 'dispx dispy dispz'
  # uniform_refine = 1
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
  [./dispx]   order = FIRST    family = LAGRANGE [../]
  [./dispy]   order = FIRST    family = LAGRANGE [../]
  [./dispz]   order = FIRST    family = LAGRANGE [../]
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
    use_displaced_mesh = true
  [../]
  
  [./ecforcing]
    type = ElectrocardioForcing
    variable = potential
    forcing_function = ElectrocardioForcing_function
    #ion_coeff = 0.0
    use_displaced_mesh = true
  [../]

  [./euler]
    type = ElectrocardioTimeDerivative
    variable = potential
    use_displaced_mesh = true
  [../]
[]

[Functions]
  [./ElectrocardioForcing_function]
    # we are pacing at the following "early activation sites" which were hand-picked in Paraview
    # to closely match [Potse2006, Figure 1]:
    #   0.8 6.4 -3.1
    #  -1.4 6.4 -2.9
    #   2.2 8.9 -4.2
    type = PiecewiseParsedFunction
    default_function = '0'
    functions = '-30.0*(exp(-0.5*(pow(x-0.8,2)+pow(y-6.4,2)+pow(z+3.1,2))/pow(1.5,2))+exp(-0.5*(pow(x+1.4,2)+pow(y-6.4,2)+pow(z+2.9,2))/pow(1.5,2))+exp(-0.5*(pow(x-2.2,2)+pow(y-8.9,2)+pow(z+4.2,2))/pow(1.5,2)))'
    left      = ' 0.0'
    right     = ' 5.0'
  [../]
[]

[BCs]
  [./bc]
    type = NeumannBC
    variable = potential
    boundary = '101 102 103'
    value = 0
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
    #use_displaced_mesh = true # TODO: if activated I am getting a segfault
  [../]
 
  [./conductivity]
    type = ElectrocardioConductivity
    conductivities = '0.006 0.006 0.006'
    block = all
    #use_displaced_mesh = true # TODO: if activated I am getting a segfault
  [../]
[]

[Executioner]
  type = Transient   # Here we use the Transient Executioner
  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
#l_tol = 1e-6
#  l_max_its = 10
  nl_rel_tol = 1e-3
  nl_abs_tol = 1e-6
  nl_rel_step_tol = 1e-6
  nl_max_its = 2

  start_time = 0
  end_time   = 500.0
  dtmin = 0.05
  dtmax = 0.5
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
