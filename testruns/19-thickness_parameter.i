[Mesh]
      dim           = 3
      distribution  = DEFAULT
      nx            = 5
      ny            = 5
      nz            = 20
      type          = GeneratedMesh
      xmax          =  0.5
      xmin          = -0.5
      ymax          =  0.5
      ymin          = -0.5
      zmax          =  5.0
      zmin          = -5.0
[]

[Variables]
  [./x] order = FIRST family = LAGRANGE [../]
[]

[Kernels]
  [./diff_x] type = Diffusion variable = x [../]
[]

[AuxVariables]
  [./distance] order = FIRST family = LAGRANGE [../]
[]

[AuxKernels]
  [./auxdistance]
    type = VolumeNearestNodeDistanceAux
    variable = distance
    block = 0
    paired_boundary = front
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
