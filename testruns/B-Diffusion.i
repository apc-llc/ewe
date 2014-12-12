[Mesh]
 file = 07-heart_geometry_new.e
 
 block_id = '1'
 block_name = 'all'
 
#   uniform_refine = 2
#   dim           = 2
#   distribution  = DEFAULT
#   nx            = 15
#   ny            = 15
#   nz            = 15
#   type          = GeneratedMesh
#   xmax          =  1.0
#   xmin          =  0.0
#   ymax          =  1.0
#   ymin          =  0.0
#   zmax          =  1.0
#   zmin          =  0.0
 []

[Variables]
  active = 'potential'

  [./potential]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  active = 'diff ecforcing euler'
   
  [./diff]
    type = ElectrocardioDiffusion
    variable = potential
surface_to_volume = 800.0
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

[Functions]
  [./ElectrocardioForcing_function]
    type = PiecewiseParsedFunction
    default_function = '0'
    functions = '-30.0*exp(-0.5*pow(x-0.0,2.0)/pow(0.25,2.0))'
    left      = ' 0.0'
    right     = ' 0.1'
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
   conductivities = '0.006 0.006 0.006'
   block = all
  [../]
[]

[Executioner]
  type = Transient   # Here we use the Transient Executioner
  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  l_tol = 1e-5
  l_max_its = 10
  nl_rel_tol = 1e-3
  nl_max_its = 2
  num_steps = 5
# scheme = 'implicit-euler'
# scheme ='bdf2'
 [./TimeStepper]
 type = ConstantDT
  dt = 0.1
 
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
    perf_log = true
    linear_residuals = true
  [../]
[]
