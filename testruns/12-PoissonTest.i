[Mesh]
      dim           = 3
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
  [./force_dispx]
    type      = UserForcingFunction
    variable  = dispx
    function  = force_func_dispx
  [../]
  [./diffusion_dispx]
    type      = Diffusion
    variable  = dispx
  [../]
  [./shift_dispx]
    type      = AnisotropicGradientShift
    variable  = dispx
    component = 0
  [../]

  [./force_dispy]
    type      = UserForcingFunction
    variable  = dispy
    function  = force_func_dispy
  [../]
  [./diffusion_dispy]
    type      = Diffusion
    variable  = dispy
  [../]
  [./shift_dispy]
    type      = AnisotropicGradientShift
    variable  = dispy
    component = 1
  [../]

  [./force_dispz]
    type      = UserForcingFunction
    variable  = dispz
    function  = force_func_dispz
  [../]
  [./diffusion_dispz]
    type      = Diffusion
    variable  = dispz
  [../]
  [./shift_dispz]
    type      = AnisotropicGradientShift
    variable  = dispz
    component = 2
  [../]
[]

[Functions]
  [./force_func_dispx]
    type = ParsedFunction
    value='sin(x*pi)'
  [../]
  [./force_func_dispy]
    type = ParsedFunction
    value='cos(y*pi)'
  [../]
  [./force_func_dispz]
    type = ParsedFunction
    value='exp(-z*z)'
  [../]
[]

[BCs]
   [./bc_dispx]
     type = DirichletBC
     boundary = 'top bottom front back'
     variable = dispx
     value = 0.
   [../]
   [./bc_dispy]
     type = DirichletBC
     boundary = 'left right front back'
     variable = dispy
     value = 0.
   [../]
   [./bc_dispz]
     type = DirichletBC
     boundary = 'left right top bottom'
     variable = dispz
     value = 0.
   [../]
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true

  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
[]
