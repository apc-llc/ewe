[Mesh]
  block_id='0'
  block_name='all'
  displacements='dispx dispy dispz'
  dim           = 3
  distribution  = DEFAULT
  nx            = 32
  ny            = 8
  nz            = 8
  type          = GeneratedMesh
  xmax          = 10.0
  xmin          = 0.0
  ymax          = 2.5
  ymin          = 0.0
  zmax          = 2.5
  zmin          = 0.0
  elem_type = HEX20
[]

[Variables]
  [./potential]  order=FIRST  family=LAGRANGE  [../]
[]

[AuxVariables]
  [./dispx]    order=SECOND     family = LAGRANGE  [../]
  [./dispy]    order=SECOND     family = LAGRANGE  [../]
  [./dispz]    order=SECOND     family = LAGRANGE  [../]
[]

[AuxKernels]
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
    type = PiecewiseFunction
    default_function = '0'
    functions = 'PurkinjePacing'
    left      = ' 0.0'
    right     = ' 5.0'
  [../]

  [./PurkinjePacing]
    type = ParsedFunction
    value = 'A*( exp(-0.5*(pow(x-0.0,2)+pow(y-0.0,2)+pow(z-1.25,2))/pow(w,2)) )'
    vars = '  A   w' # amplitude and spatial width of the Gaussians
    vals = '-30 0.75'
  [../]
[]

[BCs]
  [./bc]
    type = NeumannBC
    variable = potential
    boundary = '1 2 3 4 5'
    value = 0
  [../]
[]
 
[ICs]
  [./ec]
    type = ElectrocardioIC
    variable = potential
    block = all
  [../]
[]

[Materials]
   [./fibres]
     type=CardiacFibresMaterial
     block=all
     fixed_R='1 0 0
     0 1 0
     0 0 1'
     outputs=all
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
  type=Transient

  solve_type=PJFNK
# petsc_options_iname='-snes_type -ksp_type -pc_type -pc_factor_shift_type '
#  petsc_options_value=' newtonls   preonly   lu       NONZERO'

  nl_rel_tol=1e-9
  nl_abs_tol=1e-9
  nl_rel_step_tol=1e-9
  nl_abs_step_tol=1e-9

  l_tol=1.e-6
  l_max_its=30
  #l_abs_step_tol=1.e-12
  #l_max_its=20

  start_time=  0.0
  end_time  = 10.0
  dtmin     =  0.1
  dtmax     =  0.2
[]

 
[Outputs]
  output_on = 'initial timestep_end'
  exodus=true
  [./console]
    type=Console
    perf_log=false
    output_on = 'initial nonlinear timestep_begin timestep_end'
  [../]
 []
