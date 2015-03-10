[Mesh]
  file = 'ventricle_fine.e'
  block_id = '1'
  block_name = 'all'
  uniform_refine = 1
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
  [./diff] type=ElectrocardioDiffusion variable=potential  surface_to_volume=1000.0  capacitance=1.0  use_displaced_mesh=true [../]
  
  [./ecforcing] type=ElectrocardioForcing  variable=potential  forcing_function=ElectrocardioForcing_function  use_displaced_mesh=true [../]

  [./euler]
    type = ElectrocardioTimeDerivative
    variable = potential
    use_displaced_mesh = true
  [../]
[]

[Functions]
  [./PurkinjePacing]
    type = ParsedFunction
    value = 'A*( exp(-0.5*(pow(x-1.0, 2)+pow(y-1.0, 2)+pow(z-0.5,2))/pow(w,2)) )'
    vars = '  A   w' # amplitude and spatial width of the Gaussians
    vals = '-30 0.4'
  [../]

  [./ElectrocardioForcing_function]
    type = PiecewiseFunction
    default_function = '0'
    functions = 'PurkinjePacing PurkinjePacing PurkinjePacing'
    left      = '0.0 350.0 700.0'
    right     = '5.0 355.0 705.0'
  [../]
[]

[BCs]
  [./potential] type = NeumannBC  variable = potential boundary = '101 103' value = 0 [../]
[]

[Materials]
 [./fibres]
    type = CardiacFibresMaterial
    fixed_R='1 0 0
             0 1 0
             0 0 1'
    outputs = all
    block = all
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
    conductivities = '0.001 0.001 0.001'
    block = all
    #use_displaced_mesh = true # TODO: if activated I am getting a segfault
  [../]
[]

[Executioner]
  type = Transient   # Here we use the Transient Executioner
  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
#petsc_options = '-snes_ksp_ew_conv' # use variable tolerance in linear solver to avoid over-solving the linear system
  #l_tol = 1e-6
  #l_max_its = 10
  nl_rel_tol = 1e-4
  nl_abs_tol = 1e-7
  nl_rel_step_tol = 1e-6
  #nl_max_its = 2

  start_time = 0
  end_time   = 1000.0
  dtmin = 0.05
  dtmax = 0.5
  scheme = 'implicit-euler'
[]

[ICs]
  [./ec] type = ElectrocardioIC  variable = potential  block = all  [../]
[]
 
[Outputs]
  [./console] type = Console  perf_log = false output_on = 'initial timestep_end' [../]
  [./out] type=Exodus output_on = 'initial timestep_end' interval = 10 [../]
[]
