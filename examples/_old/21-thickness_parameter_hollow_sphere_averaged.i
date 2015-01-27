[Mesh]
  file = 21-hollow_sphere.e
[]

[Variables]
  [./x] order = FIRST family = LAGRANGE [../]
[]

[Kernels]
  [./diff_x] type = Diffusion variable = x [../]
[]

[AuxVariables]
  [./distance_outer] order = FIRST family = LAGRANGE [../]
  [./distance_inner] order = FIRST family = LAGRANGE [../]
   [./thickness_parameter] order = FIRST family = LAGRANGE [../]
  [./thickness_parameter_averaged] order = FIRST family = LAGRANGE [../]
[]

[AuxKernels]
  [./auxdistance_outer]
    type = VolumeNearestNodeDistanceAux
    variable = distance_outer
    block = 1
    paired_boundary = ss_outer
  [../]

  [./auxdistance_inner]
    type = VolumeNearestNodeDistanceAux
    variable = distance_inner
    block = 1
    paired_boundary = ss_inner
  [../]

  [./auxthickness]
    type = CardiacThicknessParameterAux
    variable = thickness_parameter
    distance_RV_inner = distance_inner
    distance_LV_inner = distance_inner
    distance_outer    = distance_outer
  [../]

  [./auxthickness_averaged]
    type = NeighborElementAverageAux
    variable = thickness_parameter_averaged
    input_variable = thickness_parameter
  [../]
[]

[Materials]
  [./fibres]
    type = CardiacFibresMaterial
    block = 1
    thickness_parameter = thickness_parameter_averaged
    outputs = all
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
