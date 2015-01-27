[Mesh]
  file = 07-heart_geometry_new.e
[]

[Variables]
  [./x] order = FIRST family = LAGRANGE [../]
[]

[Kernels]
  [./diff_x] type = Diffusion variable = x [../]
[]

[AuxVariables]
  [./distance_outer]    order = FIRST family = LAGRANGE [../]
  [./distance_RV_inner] order = FIRST family = LAGRANGE [../]
  [./distance_LV_inner] order = FIRST family = LAGRANGE [../]
  [./thickness_parameter] order = FIRST family = LAGRANGE [../]
[]

[AuxKernels]
  [./auxdistance_outer]
    type = VolumeNearestNodeDistanceAux
    variable = distance_outer
    block = 1
    paired_boundary = ss_outer
  [../]

  [./auxdistance_RV_inner]
    type = VolumeNearestNodeDistanceAux
    variable = distance_RV_inner
    block = 1
    paired_boundary = ss_RV_inner
  [../]

  [./auxdistance_LV_inner]
    type = VolumeNearestNodeDistanceAux
    variable = distance_LV_inner
    block = 1
    paired_boundary = ss_LV_inner
  [../]

  [./auxthickness]
    type = CardiacThicknessParameterAux
    variable = thickness_parameter
    distance_RV_inner = distance_RV_inner
    distance_LV_inner = distance_LV_inner
    distance_outer    = distance_outer
  [../]
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
  
  [./console]
    type = Console
    perf_log = false
    linear_residuals = false
  [../]
[]
