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
      #elem_type     =  HEX20
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
  [./incomp_dispx]
    type      = CardiacKirchhoffIncompressibilityPenalty
    use_displaced_mesh = false
    variable  = dispx
    component = 0
    dispx     = dispx
    dispy     = dispy
    dispz     = dispz
    gamma     = 1.e3
  [../]

  [./stressdiv_dispy]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispy
    component = 1
    displacements ='dispx dispy dispz'
  [../]
  [./incomp_dispy]
    type      = CardiacKirchhoffIncompressibilityPenalty
    use_displaced_mesh = false
    variable  = dispy
    component = 1
    dispx     = dispx
    dispy     = dispy
    dispz     = dispz
    gamma     = 1.e3
  [../]

  [./stressdiv_dispz]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispz
    component = 2
    displacements ='dispx dispy dispz'
  [../]
  [./incomp_dispz]
    type      = CardiacKirchhoffIncompressibilityPenalty
    use_displaced_mesh = false
    variable  = dispz
    component = 1
    dispx     = dispx
    dispy     = dispy
    dispz     = dispz
    gamma     = 1.e3
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
    displacements ='dispx dispy dispz'
    outputs     = all
    output_properties = 'Kirchhoff_stress'
    Ta_function = active_tension
    #p = hydrostatic_pressure
  [../]
[]

[Functions]
  [./active_tension]
    type = PiecewiseLinear
    x = '0.0  1.0'
    y = '0.0  1.0'
    scale_factor = 1
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
  [./elastic_energy]
    type = ElementIntegralMaterialProperty
    execute_on = timestep
    mat_prop = elastic_energy_density
  [../]

  [./volume_ratio]
    type = CardiacMaterialVolumeRatioPostprocessor
    execute_on = residual
  [../]
[]

[Preconditioning]
  [./fdp]
    type = FDP
  [../]
[]

[Executioner]
  type = Transient

  solve_type = FD
  #petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  #petsc_options_value = '201                 hypre    boomeramg      4'
  #line_search = 'none'


  #nl_rel_step_tol = 1.e-8
  #l_max_its = 100

  start_time = 0
  end_time   = 1.0
  #num_steps = 10
  dtmax      = 0.05
  dtmin      = 0.05
[]

[Outputs]
  exodus = true

  [./console]
    type = Console
    perf_log = false
    linear_residuals = false
  [../]
[]
