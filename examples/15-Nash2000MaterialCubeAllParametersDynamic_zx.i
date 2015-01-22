[Mesh]
      dim           = 3
      nx            = 1
      ny            = 1
      nz            = 1
      type          = GeneratedMesh
      xmax          =  1.0
      xmin          =  0.0
      ymax          =  1.0
      ymin          =  0.0
      zmax          =  1.0
      zmin          =  0.0
[]

[Variables]
  [./dispx]
    order = FIRST
    family = LAGRANGE
  [../]

  [./dispy]
    order = FIRST
    family = LAGRANGE
  [../]

  [./dispz]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./stressdiv_dispx]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispx
    component = 0
    displacements ='dispx dispy dispz'
  [../]

  [./stressdiv_dispy]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispy
    component = 1
    displacements ='dispx dispy dispz'
  [../]

  [./stressdiv_dispz]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispz
    component = 2
    displacements ='dispx dispy dispz'
  [../]
[]

[Materials]
  [./cardiac_properties]
    type = CardiacFibresMaterial
    fixed_R = '1 0 0 0 1 0 0 0 1'
    block = 0
    outputs = all
  [../]

  [./cardiac_material]
    type = CardiacNash2000Material
    block = 0
    use_displaced_mesh = false
    # material parameters in the order 11 22 33 12 23 31 (symmetric)
    # taken from [Nash & Hunter, 2000], Table I
    k_MN = '1.937 0.028 0.310 1.000 1.000 1.000'
    a_MN = '0.523 0.681 1.037 0.731 0.886 0.731'
    b_MN = '1.351 5.991 0.398 2.000 2.000 2.000'
    displacements ='dispx dispy dispz'
    outputs    = all
    output_properties = 'Kirchhoff_stress'
  [../]
[]

[Functions]
  [./pull]
    type = PiecewiseLinear
    x = '0.0  1.0'
    y = '0.0 -2.5'
    scale_factor = 1
  [../]
[]

[BCs]
   [./bc_pull]
     type = FunctionDirichletBC
     boundary = 'back'
     variable = dispx
     function = pull
   [../]
   [./bc_fixy]
     type = DirichletBC
     boundary = 'back'
     variable = dispy
     value = 0.
   [../]
   [./bc_fixz]
     type = DirichletBC
     boundary = 'back'
     variable = dispz
     value = 0.
   [../]

   [./bc_dispx]
     type = DirichletBC
     boundary = 'front'
     variable = dispx
     value = 0.
   [../]
   [./bc_y]
     type = DirichletBC
     boundary = 'front'
     variable = dispy
     value = 0.
   [../]
   [./bc_dispz]
     type = DirichletBC
     boundary = 'front'
     variable = dispz
     value = 0.
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
  end_time   = 1.0
  #num_steps = 10
  dtmax      = 0.005
  dtmin      = 0.005
[]

[Outputs]
  exodus = true

  [./console]
    type = Console
    perf_log = false
    linear_residuals = true
  [../]
[]
