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
  [./stressdiv_dispx]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispx
    component = 0
    dispx     = dispx
    dispy     = dispy
    dispz     = dispz
  [../]

  [./force_dispy]
    type      = UserForcingFunction
    variable  = dispy
    function  = force_func_dispy
  [../]
  [./stressdiv_dispy]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispy
    component = 1
    dispx     = dispx
    dispy     = dispy
    dispz     = dispz
  [../]

  [./force_dispz]
    type      = UserForcingFunction
    variable  = dispz
    function  = force_func_dispz
  [../]
  [./stressdiv_dispz]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispz
    component = 2
    dispx         = dispx
    dispy         = dispy
    dispz         = dispz
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

[Materials]
  [./cardiac_properties]
    type = CardiacPropertiesMaterial
    block = 0
    outputs = all
  [../]

  [./cardiac_material]
    type = CardiacWhiteley2007Material
    block = 0
    use_displaced_mesh = false
    # material parameters in the order 11 22 33 12 23 31 (symmetric)
    # taken from [Nash & Hunter, 2000], Table I
    k_MN = '1.937 0.028 0.310 1.000 1.000 1.000'
    a_MN = '0.523 0.681 1.037 0.731 0.886 0.731'
    b_MN = '1.351 5.991 0.398 2.000 2.000 2.000'
    dispx      = dispx
    dispy      = dispy
    dispz      = dispz
  [../]
[]

[BCs]
   [./bc_dispx]
     type = DirichletBC
     boundary = 'top bottom front back'
     variable = dispx
     value = 0.
   [../]
   [./bc_y]
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