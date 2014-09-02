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
      nz                         = 5                          # Number of elements in the Z direction
      partitioner                = default                     # Specifies a mesh partitioner to use when splitting the mesh for a parallel ...
                                                               # computation.
      type                       = GeneratedMesh
      xmax                       =  0.5                           # Upper X Coordinate of the generated mesh
      xmin                       = -0.5                           # Lower X Coordinate of the generated mesh
      ymax                       =  0.5                           # Upper Y Coordinate of the generated mesh
      ymin                       = -0.5                           # Lower Y Coordinate of the generated mesh
      zmax                       =  0.5                           # Upper Z Coordinate of the generated mesh
      zmin                       = -0.5                           # Lower Z Coordinate of the generated mesh
  displacements='x_disp y_disp z_disp'
[]

[Variables]
  [./x_disp]
    order = FIRST
    family = LAGRANGE
  [../]

  [./y_disp]
    order = FIRST
    family = LAGRANGE
  [../]

  [./z_disp]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[SolidMechanics]
  [./solid]
    disp_x = x_disp
    disp_y = y_disp
    disp_z = z_disp
  [../]
[]

[Kernels]
  active='xtime ytime ztime'

  [./xtime]
    type = SolidMechImplicitEuler
    variable = x_disp
  [../]

  [./ytime]
    type = SolidMechImplicitEuler
    variable = y_disp
  [../]

  [./ztime]
    type = SolidMechImplicitEuler
    variable = z_disp
  [../]
[]


[BCs]

  [./x_force]
    type = NeumannBC
    variable = x_disp
    boundary = 'left right'
    value = 0.0
  [../]

  [./y_force]
    type = NeumannBC
    variable = y_disp
    boundary = 'top bottom'
    value = 0.0
  [../]


  [./z_force]
    type = NeumannBC
    variable = z_disp
    boundary = 'front back'
    value = 0.0
  [../]


  [./z_compression_bottom]
    type = DirichletBC
    variable = z_disp
    boundary = front
    value = 0.0
  [../]

  [./z_compression_top]
    type = FunctionDirichletBC
    variable = z_disp
    boundary = back
    function = pull
  [../]
[]

[Materials]
  [./constant]
    type = LinearIsotropicMaterial
    block = 0
    youngs_modulus = 0.01e6
    poissons_ratio = 0.3
    disp_x = x_disp
    disp_y = y_disp
    disp_z = z_disp
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


  nl_rel_step_tol = 1.e-13
  l_max_its = 100

  start_time = 0
  end_time = 1
#  num_steps = 5000
  dtmax = 0.1
  dtmin = 0.1

  [./TimeStepper]
    type = ConstantDT
    dt = 0.1
#    optimal_iterations = 12
#    linear_iteration_ratio = 100
#    time_t  = '-100 0' # direct control of time steps vs time (optional)
#    time_dt = '100  900'
  [../]
[]

[Functions]
  [./pull]
    type = PiecewiseLinear
    x = '0.0 0.1      0.2     0.3    0.4        0.5      0.6   0.7      0.8    0.9    1.0'
    y = '0.0 0.000167 0.00133 0.0045 0.010667   0.020833 0.036 0.057167 0.0853 0.1215 0.16667'
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
