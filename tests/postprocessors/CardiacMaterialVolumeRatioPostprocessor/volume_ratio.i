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
[]

[Kernels]
  [./stressdiv_dispx]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispx
    component = 0
    displacements = 'dispx dispy dispz'
  [../]

  [./stressdiv_dispy]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispy
    component = 1
    displacements = 'dispx dispy dispz'
  [../]

  [./stressdiv_dispz]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispz
    component = 2
    displacements = 'dispx dispy dispz'
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
    displacements = 'dispx dispy dispz'
  [../]
[]

[BCs]
   [./bc_pull]
     type = DirichletBC
     boundary = 'left'
     variable = dispx
     value = 0.25
   [../]
   [./bc_fixy]
     type = DirichletBC
     boundary = 'left'
     variable = dispy
     value = 0.
   [../]
   [./bc_fixz]
     type = DirichletBC
     boundary = 'left'
     variable = dispz
     value = 0.
   [../]

   [./bc_dispx]
     type = DirichletBC
     boundary = 'right'
     variable = dispx
     value = 0.
   [../]
   [./bc_y]
     type = DirichletBC
     boundary = 'right'
     variable = dispy
     value = 0.
   [../]
   [./bc_dispz]
     type = DirichletBC
     boundary = 'right'
     variable = dispz
     value = 0.
   [../]
[]

[Executioner]
  type = Steady
[]

[Postprocessors]
  [./volume_ratio]
    type = CardiacMaterialVolumeRatioPostprocessor
    execute_on = residual
    outputs = all
  [../]
[]

[Outputs]
  csv = true

  [./console]
    type = Console
    perf_log = false
    linear_residuals = true
  [../]
[]
