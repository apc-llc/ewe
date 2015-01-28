# use with ewe-opt

[Mesh]
      dim           = 3
      distribution  = DEFAULT
      nx            = 1
      ny            = 1
      nz            = 1
      type          = GeneratedMesh
      xmax          =  0.5
      xmin          = -0.5
      ymax          =  0.5
      ymin          = -0.5
      zmax          =  0.5
      zmin          = -0.5
[]

[Variables]
  [./var] [../]
[]

[Kernels]
  [./kern] type=Diffusion variable=var [../]
[]

[Materials]
  [./Tests]
    type=CardiacTestsMaterial
    block=0
  [../]
[]

[BCs]
[]

[Functions]
[]


[Executioner]
  type = Steady
[]

[Outputs]
  [./console]
    type = Console
    perf_log = false
    output_on = ''
  [../]
[]
