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
  [./x]
    order = FIRST
    family = LAGRANGE
  [../]

  [./y]
    order = FIRST
    family = LAGRANGE
  [../]

  [./z]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./force_x]
    type      = UserForcingFunction
    variable  = x
    function  = force_func_x
  [../]
  [./stressdiv_x]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = x
    component = 0
    x         = x
    y         = y
    z         = z
  [../]

  [./force_y]
    type      = UserForcingFunction
    variable  = y
    function  = force_func_y
  [../]
  [./stressdiv_y]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = y
    component = 1
    x         = x
    y         = y
    z         = z
  [../]

  [./force_z]
    type      = UserForcingFunction
    variable  = z
    function  = force_func_z
  [../]
  [./stressdiv_z]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = z
    component = 2
    x         = x
    y         = y
    z         = z
  [../]
[]

[Functions]
  [./force_func_x]
    type = ParsedFunction
    value='sin(x*pi)'
  [../]
  [./force_func_y]
    type = ParsedFunction
    value='cos(y*pi)'
  [../]
  [./force_func_z]
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
    # These lead to T(MN)=delta(MN)
    k_MN = '-1. -1. -1.  0.  0.  0.'
    a_MN = ' 0.  0.  0.  0.  0.  0.'
    b_MN = ' 1.  1.  1.  1.  1.  1.'
    x      = x
    y      = y
    z      = z
  [../]
[]

[BCs]
   [./bc_x]
     type = DirichletBC
     boundary = 'top bottom front back'
     variable = x
     value = 0.
   [../]
   [./bc_y]
     type = DirichletBC
     boundary = 'left right front back'
     variable = y
     value = 0.
   [../]
   [./bc_z]
     type = DirichletBC
     boundary = 'left right top bottom'
     variable = z
     value = 0.
   [../]
[]

[AuxVariables]
  [./dispx] order=FIRST family=LAGRANGE [../]
  [./dispy] order=FIRST family=LAGRANGE [../]
  [./dispz] order=FIRST family=LAGRANGE [../]
[]

[AuxKernels]
  [./aux_dispx] type=DisplacementAux component=0 variable=dispx coordinate=x [../]
  [./aux_dispy] type=DisplacementAux component=1 variable=dispy coordinate=y [../]
  [./aux_dispz] type=DisplacementAux component=2 variable=dispz coordinate=z [../]
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