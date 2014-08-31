#Run with 4 procs

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
[]

#[BCs]
#
#  [./y_force]
#    type = NeumannBC
#    variable = y_disp
#    boundary = 2
#    value = -1.0
#  [../]
#
#  [./left]
#    type = DirichletBC
#    variable = x_disp
#    boundary = 3
#    value = 0.0
#  [../]
#
#  [./bottom]
#    type = DirichletBC
#    variable = y_disp
#    boundary = 1
#    value = 0.0
#  [../]
#
#[]

[Materials]
  [./constant]
    type = LinearIsotropicMaterial
    block = 1
    # material parameters in the order 11 22 33 12 23 31 (symmetric), taken from [Nash & Hunter, 2000], Table I
    k_MN = 1.937 0.028 0.310 1.000 1.000 1.000
    a_MN = 0.523 0.681 1.037 0.731 0.886 0.731
    b_MN = 1.351 5.991 0.398 2.000 2.000 2.000
    x = x                            # connection between coupled variables in the material and our simulation variables for x
    y = y                            # .. y
    z = z                            # .. z
    p = p                            # .. p (pressure / Lagrange multiplier for incompressibility)
    #TODO: add this Ta = Ta                          # .. active strain, i.e. strain that results from electrophysiology
  [../]
[]

[Kernels]
  [./dummy_x]
    type = Diffusion
    variable = x
  [../]

  [./dummy_y]
    type = Diffusion
    variable = y
  [../]

  [./dummy_z]
    type = Diffusion
    variable = z
  [../]
[]

[Executioner]
  type = Steady

  solve_type = PJFNK

  l_max_its = 100

#  start_time = 0.0
#  num_steps = 4
#  dt = 0.000005
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
