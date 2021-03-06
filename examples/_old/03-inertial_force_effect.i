# use with moose/modules/solid_mechanics/solid_mechanics-opt

[Mesh]
      dim           = 3
      distribution  = DEFAULT
      nx            = 5
      ny            = 5
      nz            = 20
      type          = GeneratedMesh
      xmax          =  0.5
      xmin          = -0.5
      ymax          =  0.5
      ymin          = -0.5
      zmax          =  5.0
      zmin          = -5.0
      displacements = 'disp_x disp_y disp_z'
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
    type     = SecondOrderImplicitEulerWithDensity
    variable = disp_x
    density  = 100.0
    lumping  = false
  [../]
  [./inertia_y]
    type     = SecondOrderImplicitEulerWithDensity
    variable = disp_y
    density  = 100.0
    lumping  = false
  [../]
  [./inertia_z]
    type     = SecondOrderImplicitEulerWithDensity
    variable = disp_z
    density  = 100.0
    lumping  = false
  [../]
[]


[BCs]
  [./force_x]
    type = NeumannBC
    variable = disp_x
    boundary = 'front'
    value = 0.0
  [../]
  [./force_y]
    type = NeumannBC
    variable = disp_y
    boundary = 'front'
    value = 0.0
  [../]
  [./force_z]
    type = NeumannBC
    variable = disp_z
    boundary = 'front'
    value = 0.0
  [../]

  [./displacement_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'front'
    value = 0.0
  [../]
  [./displacement_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'front'
    value = 0.0
  [../]
  [./displacement_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'front'
    value = 0.0
  [../]

  [./force_back]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'back'
    function = pull
  [../]
[]

[Materials]
  [./linear_isotropic]
    type = LinearIsotropicMaterial
    block = 0
    youngs_modulus = 1.e5
    poissons_ratio = 0.3
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]

[]

[Executioner]
  type = Transient

  solve_type = PJFNK
  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                 hypre    boomeramg      4'
  line_search = 'none'


  nl_rel_step_tol = 1.e-8
  l_max_its = 100

  start_time = 0
  end_time   = 3.0
  #num_steps = 10
  dtmax      = 0.1
  dtmin      = 0.01
[]

[Functions]
  [./pull]
    type = PiecewiseLinear
    x = '0.0 0.5 0.51 100.0'
    y = '0.0 1.0 0.0    0.0'
    scale_factor = 1
  [../]
[]

[Outputs]
  exodus = true
  
  [./console]
    type = Console
    perf_log = true
    linear_residuals = false
  [../]
  
  [./exodus_displaced]
     file_base = out
     type = Exodus
     use_displaced = true
  [../]  
[]
