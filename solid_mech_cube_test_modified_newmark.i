# use with moose/modules/solid_mechanics/solid_mechanics-opt

[Mesh]
      dim                        = 3                           # The dimension of the mesh to be generated
      distribution               = DEFAULT                     # PARALLEL: Always use libMesh::ParallelMesh SERIAL: Always use libMesh::SerialMesh ...
                                                               # DEFAULT: Use libMesh::SerialMesh unless --parallel-mesh is specified ...
                                                               # on the command line
      # elem_type                =                             # The type of element from libMesh to generate (default: linear element ...
                                                               # for requested dimension)
      nx                         = 5                          # Number of elements in the X direction
      ny                         = 5                          # Number of elements in the Y direction
      nz                         = 50                         # Number of elements in the Z direction
      partitioner                = default                     # Specifies a mesh partitioner to use when splitting the mesh for a parallel ...
                                                               # computation.
      type                       = GeneratedMesh
      xmax                       =  0.5                           # Upper X Coordinate of the generated mesh
      xmin                       = -0.5                           # Lower X Coordinate of the generated mesh
      ymax                       =  0.5                           # Upper Y Coordinate of the generated mesh
      ymin                       = -0.5                           # Lower Y Coordinate of the generated mesh
      zmax                       =  5.0                           # Upper Z Coordinate of the generated mesh
      zmin                       = -5.0                           # Lower Z Coordinate of the generated mesh
  displacements='disp_x disp_y disp_z'
[]

[Variables]
  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]

  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]

  [./disp_z]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./vel_x]  [../]
  [./vel_y]  [../]
  [./vel_z]  [../]
  [./accel_x]  [../]
  [./accel_y]  [../]
  [./accel_z]  [../]
[]

[SolidMechanics]
  # for every variable given, this sets up a StressDivergence kernel (see SolidMechanicsAction.C)  
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]
[]

[Kernels]
  [./inertia_x]
    type = SolidMechInertialForceNew
    variable     = disp_x
    acceleration = accel_x
    beta  = 0.25
  [../]
  [./inertia_y]
    type = SolidMechInertialForceNew
    variable     = disp_y
    acceleration = accel_y
    beta  = 0.25
  [../]
  [./inertia_z]
    type = SolidMechInertialForceNew
    variable     = disp_z
    acceleration = accel_z
    beta  = 0.25
  [../]
[]

[AuxKernels]
  [./accel_x]
    type = NewmarkAccelAux
    variable     = accel_x
    displacement = disp_x
    velocity     = vel_x
    beta       = 0.25
    execute_on = timestep
  [../]
  [./accel_y]
    type = NewmarkAccelAux
    variable     = accel_y
    displacement = disp_y
    velocity     = vel_y
    beta       = 0.25
    execute_on = timestep
  [../]
  [./accel_z]
    type = NewmarkAccelAux
    variable     = accel_z
    displacement = disp_z
    velocity     = vel_z
    beta       = 0.25
    execute_on = timestep
  [../]
  [./vel_x]
    type = NewmarkVelAux
    variable     = vel_x
    acceleration = accel_x
    gamma      = 0.5
    execute_on = timestep
  [../]
  [./vel_y]
    type = NewmarkVelAux
    variable     = vel_y
    acceleration = accel_y
    gamma      = 0.5
    execute_on = timestep
  [../]
  [./vel_z]
    type = NewmarkVelAux
    variable     = vel_z
    acceleration = accel_z
    gamma      = 0.5
    execute_on = timestep
  [../]
[]

[BCs]

  [./force_x]
    type = NeumannBC
    variable = disp_x
    boundary = 'left right top bottom front back'
    value = 0.0
  [../]
  [./force_y]
    type = NeumannBC
    variable = disp_y
    boundary = 'left right top bottom front back'
    value = 0.0
  [../]
  [./force_z]
    type = NeumannBC
    variable = disp_z
    boundary = 'left right top bottom front back'
    value = 0.0
  [../]

  [./displacement_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'back'
    value = 0.0
  [../]
  [./displacement_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'back'
    value = 0.0
  [../]

  [./moving_y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'back'
    function = pull
  [../]
[]

[Materials]
  [./constant]
    type = LinearIsotropicMaterial
    block = 0
    youngs_modulus = 0.01e6
    poissons_ratio = 0.3
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]

  [./density]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'density'
    prop_values = '0.02'
  [../]
[]

[Executioner]
  type = Transient

  solve_type = PJFNK
  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                hypre    boomeramg      4'
  line_search = 'none'


  nl_rel_step_tol = 1.e-8
  l_max_its = 100

  start_time = 0
  end_time = 100
#  num_steps = 5000
  dtmax = 0.1
  dtmin = 0.01

  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 0.1
    adapt_log = True
  [../]
[]

[Functions]
  [./pull]
    type = PiecewiseLinear
    x = '0.0 1.0 100.0'
    y = '0.0 3.0   3.0'
    scale_factor = 1
  [../]
[]

[Outputs]
  exodus = true
  
  [./console]
    type = Console
    perf_log = true
    #linear_residuals = true
  [../]
  
  [./exodus_displaced]
     file_base = out
     type = Exodus
     use_displaced = true
  [../]  
[]
