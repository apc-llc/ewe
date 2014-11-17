[Mesh]
#file = 07-heart_geometry_new.e
 
 block_id = '0'
 block_name = 'all'
 
# uniform_refine = 3
 dim           = 2
 distribution  = DEFAULT
 nx            = 100
 ny            = 100
 nz            = 3
 type          = GeneratedMesh
 xmax          = 5.0
 xmin          = 0.0
 ymax          = 5.0
 ymin          = 0.0
 zmax          = 5.0
 zmin          = 0.0
 []

[Variables]
  active = 'potential'

  [./potential]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./gate_m]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./gate_v]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./gate_f]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./gate_to]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./gate_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]

  [./auxgate_m]
    type = MaterialStdVectorAux
    property = 'gates'
    variable = gate_m
    block = all
    index = 0
  [../]
 
   [./auxgate_v]
     type = MaterialStdVectorAux
     property = 'gates'
     variable = gate_v
     block = all
     index = 1
   [../]
 
   [./auxgate_f]
     type = MaterialStdVectorAux
     property = 'gates'
     variable = gate_f
     block = all
     index = 2
   [../]
 
   [./auxgate_to]
     type = MaterialStdVectorAux
     property = 'gates'
     variable = gate_to
     block = all
     index = 3
   [../]
   
   [./auxgate_x]
     type = MaterialStdVectorAux
     property = 'gates'
     variable = gate_x
     block = all
     index = 4
   [../]
 
[]

[Kernels]
  active = 'diff ecforcing euler'
   
  [./diff]
    type = ElectrocardioDiffusion
    variable = potential
    surface_to_volume = 1000.0
    capacitance = 1.0
  [../]
  
  [./ecforcing]
      type = ElectrocardioForcing
      variable = potential
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
    boundary = '1 2 3'
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
  [../]
 
  [./conductivity]
   type = ElectrocardioConductivity
   conductivity_coefficient = 0.006
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
# num_steps = 10
  num_steps = 5000
  scheme = 'implicit-euler'
# scheme ='bdf2'
 [./TimeStepper]
  type = ConstantDT
  dt = 0.05
 [../]
[]

[ICs]
  [./ec]
    type = ElectrocardioIC
    variable = potential
    block = all
  [../]
[]
 
[Outputs]
  file_base = out
  exodus = true
  [./console]
    type = Console
    perf_log = false
    linear_residuals = false
  [../]
[]
