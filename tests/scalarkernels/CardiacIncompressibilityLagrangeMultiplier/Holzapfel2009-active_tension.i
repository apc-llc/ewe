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

  [./hydrostatic_pressure]
    order = FIRST
    family = SCALAR
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

[ScalarKernels]
  [./incompressibility]
    type = CardiacIncompressibilityLagrangeMultiplier
    variable = hydrostatic_pressure
    volume_ratio_postprocessor = volume_ratio
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
    Ta_function = active_tension
    p = hydrostatic_pressure
  [../]
[]

[Functions]
  [./active_tension]
    type = ConstantFunction
    value = 15.0
  [../]
[]

[BCs]
   [./bc_dispx]
     type = DirichletBC
     boundary = 'left'
     variable = dispx
     value = 0.
   [../]
   [./bc_y]
     type = DirichletBC
     boundary = 'left'
     variable = dispy
     value = 0.
   [../]
   [./bc_dispz]
     type = DirichletBC
     boundary = 'left'
     variable = dispz
     value = 0.
   [../]
[]

[Postprocessors]
  [./volume_ratio]
    type = CardiacMaterialVolumeRatioPostprocessor
    execute_on = residual
  [../]
[]

[Executioner]
  type = Steady

  solve_type = PJFNK
  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                 hypre    boomeramg      4'
  line_search = 'none'


  nl_rel_step_tol = 1.e-8
  l_max_its = 100
[]

[Outputs]
  exodus = true

  [./console]
    type = Console
    perf_log = false
    linear_residuals = false
  [../]
[]
