[Mesh]
      dim           = 3
      distribution  = DEFAULT
      nx            = 5
      ny            = 5
      nz            = 5
      type          = GeneratedMesh
      xmax          =  0.5
      xmin          = -0.5
      ymax          =  0.5
      ymin          = -0.5
      zmax          =  0.5
      zmin          = -0.5
      elem_type     = HEX20
[]

[Variables]
  [./dispx] order = FIRST family = LAGRANGE [../]
  [./dispy] order = FIRST family = LAGRANGE [../]
  [./dispz] order = FIRST family = LAGRANGE [../]

  [./hydrostatic_pressure] order = FIRST family = SCALAR [../]
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

  #[./inertia_x] type = SecondOrderImplicitEulerWithDensity variable = dispx density  = 0.0 lumping = false [../]
  #[./inertia_y] type = SecondOrderImplicitEulerWithDensity variable = dispy density  = 0.0 lumping  = false [../]
  #[./inertia_z] type = SecondOrderImplicitEulerWithDensity variable = dispz density  = 0.0 lumping  = false [../]
[]

[ScalarKernels]
  [./incompressibility] type = CardiacIncompressibilityLagrangeMultiplier variable = hydrostatic_pressure volume_ratio_postprocessor = volume_ratio [../]
[]

[AuxVariables]
  [./distance_outer]    order = FIRST family = LAGRANGE [../]
  [./distance_RV_inner] order = FIRST family = LAGRANGE [../]
  [./distance_LV_inner] order = FIRST family = LAGRANGE [../]
  [./thickness_parameter] order = FIRST family = LAGRANGE [../]
[]

[AuxKernels]
  [./auxdistance_outer]    type = VolumeNearestNodeDistanceAux variable = distance_outer    block = 0 paired_boundary = left   [../]
  [./auxdistance_RV_inner] type = VolumeNearestNodeDistanceAux variable = distance_RV_inner block = 0 paired_boundary = top    [../]
  [./auxdistance_LV_inner] type = VolumeNearestNodeDistanceAux variable = distance_LV_inner block = 0 paired_boundary = bottom [../]

  [./auxthickness]
    type = CardiacThicknessParameterAux variable = thickness_parameter
    distance_RV_inner = distance_RV_inner
    distance_LV_inner = distance_LV_inner
    distance_outer    = distance_outer
  [../]
[]

[Materials]
  [./fibres]
    type = CardiacFibresMaterial
    block = 0
    thickness_parameter = thickness_parameter
    #fixed_R = '0.36 0.48 -0.8 -0.8 0.6 0.0 0.48 0.64 0.60'
    #fixed_R = '1 0 0 0 1 0 0 0 1'
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
    outputs = all
    output_properties = 'Kirchhoff_stress'
    Ta_function = active_tension
    p = hydrostatic_pressure
  [../]
[]

[Functions]
  [./active_tension]
    type = PiecewiseLinear
    x = '0.0  1.0'
    y = '0.0  0.5'
    scale_factor = 1
  [../]
[]

[BCs]
  [./dispx] type = DirichletBC variable = dispx boundary = 'left right top bottom front back' value = 0. [../]
  [./dispy] type = DirichletBC variable = dispy boundary = 'left right top bottom front back' value = 0. [../]
  [./dispz] type = DirichletBC variable = dispz boundary = 'left right top bottom front back' value = 0. [../]
#  [./ns_lower_polar_point_y] type = DirichletBC variable = dispy boundary = ns_lower_polar_point value = 0. [../]
#  [./ns_lower_polar_point_z] type = DirichletBC variable = dispz boundary = ns_lower_polar_point value = 0. [../]
#  [./ns_lower_polar_neighbour_x] type = DirichletBC variable = dispx boundary = ns_lower_polar_neighbour value = 0. [../]
#  [./ns_lower_polar_neighbour_y] type = DirichletBC variable = dispy boundary = ns_lower_polar_neighbour value = 0. [../]
#  [./ns_lower_polar_neighbour_z] type = DirichletBC variable = dispz boundary = ns_lower_polar_neighbour value = 0. [../]
[]

[Postprocessors]
  #[./elastic_energy] type = ElementIntegralMaterialProperty execute_on = timestep mat_prop = elastic_energy_density [../]
  [./volume_ratio] type = CardiacMaterialVolumeRatioPostprocessor execute_on = residual [../]
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = ' hypre'
  #petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  #petsc_options_value = ' 201                hypre    boomeramg      4                          '
  petsc_options = '-fp_trap -info
                   -snes_monitor -snes_view -snes_converged_reason -snes_mf_operator
                   -ksp_monitor  -ksp_view  -ksp_converged_reason  -ksp_monitor_true_residual
                   -pc_svd_monitor -snes_ls_monitor'
  line_search = 'none'

  nl_rel_tol = 1e-3
  nl_abs_tol = 1e-8
  nl_rel_step_tol = 1e-8
  l_tol = 1.e-8
  #l_max_its = 20

  start_time = 0
  end_time   = 1.0
  #num_steps = 10
  dtmax      = 0.1
  dtmin      = 0.1
[]

[Outputs]
  exodus = true

  [./console]
    type = Console
    perf_log = false
    linear_residuals = false
  [../]
[]
