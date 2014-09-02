[Debug]
  active                         = __all__                     # If specified only the blocks named will be visited and made active
  show_actions                   = 0                           # Print out the actions being executed
  show_material_props            = 1                           # Print out the material properties supplied for each block, face, neighbor, ...
                                                               # and/or sideset
  show_parser                    = 0                           # Shows parser block extraction and debugging information
  show_top_residuals             = 0                           # The number of top residuals to print out (0 = no output)
  show_var_residual_norms        = 0                           # Print the residual norms of the individual solution variables at each ...
                                                               # nonlinear iteration
  #show_var_residual              =                             # Variables for which residuals will be sent to the output file
[]

[Mesh]
      dim                        = 3                           # The dimension of the mesh to be generated
      distribution               = DEFAULT                     # PARALLEL: Always use libMesh::ParallelMesh SERIAL: Always use libMesh::SerialMesh ...
                                                               # DEFAULT: Use libMesh::SerialMesh unless --parallel-mesh is specified ...
                                                               # on the command line
      # elem_type                =                             # The type of element from libMesh to generate (default: linear element ...
                                                               # for requested dimension)
      nx                         = 20                          # Number of elements in the X direction
      ny                         = 20                          # Number of elements in the Y direction
      nz                         = 20                          # Number of elements in the Z direction
      partitioner                = default                     # Specifies a mesh partitioner to use when splitting the mesh for a parallel ...
                                                               # computation.
      type                       = GeneratedMesh
      xmax                       = 1                           # Upper X Coordinate of the generated mesh
      xmin                       = 0                           # Lower X Coordinate of the generated mesh
      ymax                       = 1                           # Upper Y Coordinate of the generated mesh
      ymin                       = 0                           # Lower Y Coordinate of the generated mesh
      zmax                       = 1                           # Upper Z Coordinate of the generated mesh
      zmin                       = 0                           # Lower Z Coordinate of the generated mesh
[]

[Variables]
  [./x]
    order = FIRST
    family = LAGRANGE
  [../]

  [./y]
    order = FIRST
    family = LAGRANGE
  [../]

  [./z]
    order = FIRST
    family = LAGRANGE
  [../]

  [./p]
    order = FIRST
    family = LAGRANGE
  [../]
  
  [./vx]
    order = FIRST
    family = LAGRANGE
  [../]

  [./vy]
    order = FIRST
    family = LAGRANGE
  [../]

  [./vz]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[BCs]
  [./force_x]
    type = ForceBC
    variable = x
    value = 0.0
    component = X
    boundary = 'left right top bottom back front'
  [../]

  [./force_y]
    type = ForceBC
    variable = y
    value = 0.0
    component = Y
    boundary = 'left right top bottom back front'
  [../]

  [./force_z]
    type = ForceBC
    variable = z
    value = 0.0
    component = Z
    boundary = 'left right top bottom back front'
  [../]

  [./disp_x]
    type = DirichletBC
    variable = x
    value = 0.0
    boundary = 'left right top bottom back front'
  [../]

  [./disp_y]
    type = DirichletBC
    variable = y
    value = 0.0
    boundary = 'left right top bottom back front'
  [../]

  [./disp_z]
    type = DirichletBC
    variable = z
    value = 0.0
    boundary = 'left right top bottom back front'
  [../]
[]

[Materials]
  [./CardiacTissueMaterial]
    block = 0
    type = CardiacTissueMaterial
    # material parameters in the order 11 22 33 12 23 31 (symmetric), taken from [Nash & Hunter, 2000], Table I
    k_MN = '1.937 0.028 0.310 1.000 1.000 1.000'
    a_MN = '0.523 0.681 1.037 0.731 0.886 0.731'
    b_MN = '1.351 5.991 0.398 2.000 2.000 2.000'
    x = x                            # connection between coupled variables in the material and our simulation variables for x
    y = y                            # .. y
    z = z                            # .. z
    p = p                            # .. p (pressure / Lagrange multiplier for incompressibility)
    #TODO: add this Ta = Ta                          # .. active strain, i.e. strain that results from electrophysiology, default (if unset) = 0
  [../]
[]

[Kernels]
  [./stress_diffusion_x]
    type      = StressDiffusion
    variable  = x
    component = X
  [../]
  [./momentum_transient_x]
    type     = MomentumTransient
    variable = x
    v        = vx
    rho = 1.05e-6                    # mass density, according to [Whiteley 2007, pg. 2204]: 1.05e-6 kPa s**2/mm**2  
  [../]

  [./stress_diffusion_y]
    type      = StressDiffusion
    variable  = y
    component = Y
  [../]
  [./momentum_transient_y]
    type     = MomentumTransient
    variable = y
    v        = vy
    rho = 1.05e-6                    # mass density, according to [Whiteley 2007, pg. 2204]: 1.05e-6 kPa s**2/mm**2  
  [../]

  [./stress_diffusion_z]
    type      = StressDiffusion
    variable  = z
    component = Z
  [../]
  [./momentum_transient_z]
    type     = MomentumTransient
    variable = z
    v        = vz
    rho = 1.05e-6                    # mass density, according to [Whiteley 2007, pg. 2204]: 1.05e-6 kPa s**2/mm**2  
  [../]

  [./position_transient_x]
    type     = PositionTransient
    variable = vx
    x        = x
  [../]

  [./position_transient_y]
    type     = PositionTransient
    variable = vy
    x        = y
  [../]

  [./position_transient_z]
    type     = PositionTransient
    variable = vz
    x        = z
  [../]

  [./incompressibility_det]
    type = IncompressibilityDeterminant
    variable = p
  [../]  
[]

[Executioner]
  type = Transient

  solve_type = PJFNK

  l_max_its = 100

  start_time = 0.0
  num_steps = 5
  dt = 0.000001
[]

[Outputs]
  file_base = cardiac_mechanics_out
  output_initial = true
  exodus = true
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
[]
