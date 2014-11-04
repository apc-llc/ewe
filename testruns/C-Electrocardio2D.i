[Mesh]
#file = 07-heart_geometry_new.e
 
 block_id = '0'
 block_name = 'all'
 
 uniform_refine = 3
 dim           = 2
 distribution  = DEFAULT
 nx            = 3
 ny            = 3
 nz            = 3
 type          = GeneratedMesh
 xmax          = 1.0
 xmin          = 0.0
 ymax          = 1.0
 ymin          = 0.0
 zmax          = 1.0
 zmin          = 0.0
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
#ion_coeff = 0.0
  [../]

  [./euler]
    type = ElectrocardioTimeDerivative
    variable = potential
    capacitance = 1.0
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
   type = CardiacPropertiesMaterial
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
   conductivity_coefficient = 2.0
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
# num_steps = 10
 num_steps = 1000
# scheme = 'implicit-euler'
# scheme ='bdf2'
 [./TimeStepper]
 type = ConstantDT
  dt = 0.001
 
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
