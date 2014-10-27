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
  [./diffusion_x]
    type      = Diffusion
    variable  = x
  [../]

  [./force_y]
    type      = UserForcingFunction
    variable  = y
    function  = force_func_y
  [../]
  [./diffusion_y]
    type      = Diffusion
    variable  = y
  [../]

  [./force_z]
    type      = UserForcingFunction
    variable  = z
    function  = force_func_z
  [../]
  [./diffusion_z]
    type      = Diffusion
    variable  = z
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

[BCs]
   [./bc_disp_x]
     type = DirichletBC
     boundary = 'top bottom front back'
     variable = x
     value = 0.
   [../]
   [./bc_disp_y]
     type = DirichletBC
     boundary = 'left right front back'
     variable = y
     value = 0.
   [../]
   [./bc_disp_z]
     type = DirichletBC
     boundary = 'left right top bottom'
     variable = z
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