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
    type = CardiacFibresMaterial
    fixed_R = '1 0 0 0 1 0 0 0 1'
    block = 0
    outputs = all
  [../]

  [./cardiac_material]
    type = CardiacNash2000Material
    block = 0
    use_displaced_mesh = false
    # material parameters in the order 11 22 33 12 23 31 (symmetric)
    # These lead to T(MN)=delta(MN)
    k_MN = '-1. -1. -1.  0.  0.  0.'
    a_MN = ' 0.  0.  0.  0.  0.  0.'
    b_MN = ' 1.  1.  1.  1.  1.  1.'
    displacements ='dispx dispy dispz'
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
