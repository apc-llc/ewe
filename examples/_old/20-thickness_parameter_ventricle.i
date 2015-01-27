[Mesh]
  file=06-heart_geometry.e
  # boundary IDs for this geometry:
  # 1 - inner
  # 2 - ring
  # 3 - outer
[]

[Variables]
  [./x] order=FIRST family=LAGRANGE [../]
[]

[Kernels]
  [./diff_x] type=Diffusion variable=x [../]
[]

[AuxVariables]
  [./distance_1] order=FIRST family=LAGRANGE [../]
  [./distance_2] order=FIRST family=LAGRANGE [../]
  [./distance_3] order=FIRST family=LAGRANGE [../]
  [./thickness_parameter] order=FIRST family=LAGRANGE [../]
[]

[AuxKernels]
  [./auxdistance_1]  type=VolumeNearestNodeDistanceAux  variable=distance_1  block=1  paired_boundary=1 [../]
  [./auxdistance_2]  type=VolumeNearestNodeDistanceAux  variable=distance_2  block=1  paired_boundary=2 [../]
  [./auxdistance_3]  type=VolumeNearestNodeDistanceAux  variable=distance_3  block=1  paired_boundary=3 [../]

  [./auxthickness]
    type=CardiacThicknessParameterAux
    variable=thickness_parameter
    distance_RV_inner=distance_1
    distance_LV_inner=distance_1
    distance_outer   =distance_3
  [../]
[]

[Executioner]
  type=Steady
[]

[Outputs]
  exodus=true
  
  [./console]
    type=Console
    perf_log=false
    linear_residuals=false
  [../]
[]
