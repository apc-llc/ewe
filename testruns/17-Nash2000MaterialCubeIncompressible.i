[Mesh]
      dim           = 3
      nx            = 1
      ny            = 1
      nz            = 1
      type          = GeneratedMesh
      xmax          =  1.0
      xmin          =  0.0
      ymax          =  1.0
      ymin          =  0.0
      zmax          =  1.0
      zmin          =  0.0
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

  [./hydrostatic_pressure]
    order = FIRST
    family = SCALAR
  [../]
[]

[Kernels]
  [./stressdiv_dispx]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispx
    component = 0
    dispx     = dispx
    dispy     = dispy
    dispz     = dispz
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

  [./stressdiv_dispz]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispz
    component = 2
    dispx     = dispx
    dispy     = dispy
    dispz     = dispz
  [../]

  [./incompressibility]
    type = CardiacKirchhoffIncompressibility
    variable = hydrostatic_pressure
    dispx    = dispx
    dispy    = dispy
    dispz    = dispz
  [../]
[]

[Materials]
  [./cardiac_properties]
    type = CardiacFibresMaterial
    fixed_R = '1 0 0 0 1 0 0 0 1'
    block = 0
  [../]

  [./cardiac_material]
    type = CardiacNash2000Material
    block = 0
    use_displaced_mesh = false
    # material parameters in the order 11 22 33 12 23 31 (symmetric)
    # taken from [Nash & Hunter, 2000], Table I
    k_MN = '1.937 0.028 0.310 1.000 1.000 1.000'
    a_MN = '0.523 0.681 1.037 0.731 0.886 0.731'
    b_MN = '1.351 5.991 0.398 2.000 2.000 2.000'
    dispx       = dispx
    dispy       = dispy
    dispz       = dispz
    outputs     = all
    output_properties = 'Kirchhoff_stress'
    #Ta_function = active_tension
    p = hydrostatic_pressure
  [../]
[]

[Functions]
  [./active_tension]
    type = PiecewiseLinear
    x = '0.0 1.0'
    y = '1.0 1.0'
    scale_factor = 1
  [../]
[]

[BCs]
   [./bc_pull]
     type = DirichletBC
     boundary = 'right'
     variable = dispx
     value = 0.2
   [../]

   [./bc_dispx]
     type = DirichletBC
     boundary = 'left'
     variable = dispx
     value = 0.
   [../]
   [./bc_y]
     type = DirichletBC
     boundary = 'left'
     variable = dispy
     value = 0.
   [../]
   [./bc_dispz]
     type = DirichletBC
     boundary = 'left'
     variable = dispz
     value = 0.
   [../]
[]

[AuxVariables]
  [./res_dispx]
    order = FIRST
    family = LAGRANGE
  []
[]

[AuxKernels]
  [./residual_dispx]
    type = DebugResidualAux
    variable = res_dispx
    debug_variable = dispx
  []
[]

[Postprocessors]
  [./elastic_energy]
    type = ElementIntegralMaterialProperty
    execute_on = timestep
    mat_prop = elastic_energy_density
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
