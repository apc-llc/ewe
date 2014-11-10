[Mesh]
      dim           = 3
      distribution  = DEFAULT
      nx            = 10
      ny            = 10
      nz            = 10
      type          = GeneratedMesh
      xmax          =  0.5
      xmin          = -0.5
      ymax          =  0.5
      ymin          = -0.5
      zmax          =  0.5
      zmin          = -0.5
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
    block = 0
    paired_boundary = left
  [../]

  [./auxdistance_RV_inner]
    type = VolumeNearestNodeDistanceAux
    variable = distance_RV_inner
    block = 0
    paired_boundary = top
  [../]

  [./auxdistance_LV_inner]
    type = VolumeNearestNodeDistanceAux
    variable = distance_LV_inner
    block = 0
    paired_boundary = bottom
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
