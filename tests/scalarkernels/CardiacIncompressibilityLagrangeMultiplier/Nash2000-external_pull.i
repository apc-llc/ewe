[Mesh]
      dim           = 3
      nx            = 3
      ny            = 3
      nz            = 3
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
    type = CardiacNash2000Material
    block = 0
    use_displaced_mesh = false
    # material parameters in the order 11 22 33 12 23 31 (symmetric)
    # taken from [Nash & Hunter, 2000], Table I
    k_MN = '1.937 0.028 0.310 1.000 1.000 1.000'
    a_MN = '0.523 0.681 1.037 0.731 0.886 0.731'
    b_MN = '1.351 5.991 0.398 2.000 2.000 2.000'
    displacements = 'dispx dispy dispz'
    p = hydrostatic_pressure
  [../]
[]

[BCs]
   [./bc_pullx]
     type = DirichletBC
     boundary = 'right'
     variable = dispx
     value = +0.001
   [../]

   [./bc_dispx]
     type = DirichletBC
     boundary = 'left'
     variable = dispx
     value = 0.
   [../]
   [./bc_dispy]
     type = DirichletBC
     boundary = 'left right'
     variable = dispy
     value = 0.
   [../]
   [./bc_dispz]
     type = DirichletBC
     boundary = 'left right'
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
