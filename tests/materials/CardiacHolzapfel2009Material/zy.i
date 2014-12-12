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
    displacements = 'dispx dispy dispz'
  [../]

  [./stressdiv_dispy]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispy
    component = 1
    displacements = 'dispx dispy dispz'
  [../]

  [./stressdiv_dispz]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispz
    component = 2
    displacements = 'dispx dispy dispz'
  [../]
[]

[Materials]
  [./cardiac_properties]
    type = CardiacFibresMaterial
    fixed_R = '1 0 0 0 1 0 0 0 1'
    block = 0
  [../]

  [./cardiac_material]
    type = CardiacHolzapfel2009Material
    block = 0
    use_displaced_mesh = false
    # material parameters as given in Table 1 of [Holzapfel 2009] in following order: a, b, a_f, b_f, a_s, b_s, a_fs, b_fs
    material_parameters = '0.059 8.023 18.472 16.026 2.481 11.120 0.216 11.436'
    displacements ='dispx dispy dispz'
    outputs    = all
    output_properties = 'Kirchhoff_stress'
  [../]
[]

[Functions]
  [./pull]
    type = PiecewiseLinear
    x = '0.0  1.0'
    y = '0.0 -1.0'
    scale_factor = 2.5
  [../]
[]

[BCs]
   [./bc_fixx]
     type = DirichletBC
     boundary = 'back'
     variable = dispx
     value = 0.
   [../]
   [./bc_pull]
     type = FunctionDirichletBC
     boundary = 'back'
     variable = dispy
     function = pull
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
