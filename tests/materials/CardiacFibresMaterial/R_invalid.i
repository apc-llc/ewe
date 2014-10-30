[Mesh]
      dim           = 1
      nx            = 1
      type          = GeneratedMesh
      xmax          =  1.0
      xmin          =  0.0
[]

[Variables]
  [./dispx]
    order = FIRST
    family = LAGRANGE
  [../]

[]

[Kernels]
  [./diff_dispx]
    type      = Diffusion
    variable  = dispx
  [../]
[]

[Materials]
  [./cardiac_properties]
    type = CardiacFibresMaterial
    fixed_R = '1 0 0 0 2 0 0 0 1'
    block = 0
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
    linear_residuals = true
  [../]
[]
