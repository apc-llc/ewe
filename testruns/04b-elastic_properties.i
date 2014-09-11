# use with moose/modules/solid_mechanics/solid_mechanics-opt

[Mesh]
      dim           = 3
      distribution  = DEFAULT
      nx            = 1
      ny            = 1
      nz            = 2
      type          = GeneratedMesh
      xmax          =  0.5
      xmin          = -0.5
      ymax          =  0.5
      ymin          = -0.5
      zmax          =  5.0
      zmin          = -5.0
      displacements = 'disp_x disp_y disp_z'
[]

[Variables]
  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]

  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]

  [./disp_z]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[SolidMechanics]
  # for every variable given, this sets up a StressDivergence kernel (see SolidMechanicsAction.C)  
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]
[]

[Kernels]
  [./inertia_x]
    type     = SecondDerivativeNewmark
    variable = disp_x
    component = X
    density  = 100.0
    lumping  = false
  [../]
  [./inertia_y]
    type     = SecondDerivativeNewmark
    variable = disp_y
    component = Y
    density  = 100.0
    lumping  = false
  [../]
  [./inertia_z]
    type     = SecondDerivativeNewmark
    variable = disp_z
    component = X
    density  = 100.0
    lumping  = false
  [../]
[]


[BCs]
  [./force_x]
    type = NeumannBC
    variable = disp_x
    boundary = 'front'
    value = 0.0
  [../]
  [./force_y]
    type = NeumannBC
    variable = disp_y
    boundary = 'front'
    value = 0.0
  [../]
  [./force_z]
    type = NeumannBC
    variable = disp_z
    boundary = 'front'
    value = 0.0
  [../]

  [./displacement_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'front'
    value = 0.0
  [../]
  [./displacement_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'front'
    value = 0.0
  [../]
  [./displacement_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'front'
    value = 0.0
  [../]

  [./force_back]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'back'
    function = pull
  [../]
[]

[Materials]
  [./linear_isotropic]
    type = LinearIsotropicMaterial
    block = 0
    youngs_modulus = 1.e5
    poissons_ratio = 0.3
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]

  [./newmark]
    type = NewmarkMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
    gamma  = 1.0
    beta   = 0.5
  [../]

[]

[Executioner]
  type = Transient

  solve_type = PJFNK
  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                 hypre    boomeramg      4'
  line_search = 'none'


  nl_rel_step_tol = 1.e-8
  l_max_its = 100

  start_time = 0
  end_time   = 3.0
  #num_steps = 10
  dtmax      = 0.1
  dtmin      = 0.01
[]

[Functions]
  [./pull]
    type = PiecewiseLinear
    x = '0.0 0.5 0.51 100.0'
    y = '0.0 1.0 0.0    0.0'
    scale_factor = 1
  [../]
[]

[Outputs]
  exodus = true
  
  [./console]
    type = Console
    perf_log = true
    linear_residuals = false
  [../]
  
  [./exodus_displaced]
     file_base = out
     type = Exodus
     use_displaced = true
  [../]  
[]


[AuxVariables]
  [./stress_xx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_zz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_xy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_yz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_zx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./elastic_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vonmises]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./hydrostatic]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./firstinv]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./secondinv]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./thirdinv]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./kinetic_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[] # AuxVariables

[AuxKernels]
  [./stress_xx]
    type = MaterialTensorAux
    tensor = stress
    variable = stress_xx
    index = 0
  [../]
  [./stress_yy]
    type = MaterialTensorAux
    tensor = stress
    variable = stress_yy
    index = 1
  [../]
  [./stress_zz]
    type = MaterialTensorAux
    tensor = stress
    variable = stress_zz
    index = 2
  [../]
  [./stress_xy]
    type = MaterialTensorAux
    tensor = stress
    variable = stress_xy
    index = 3
  [../]
  [./stress_yz]
    type = MaterialTensorAux
    tensor = stress
    variable = stress_yz
    index = 4
  [../]
  [./stress_zx]
    type = MaterialTensorAux
    tensor = stress
    variable = stress_zx
    index = 5
  [../]
  [./elastic_energy]
    type = ElasticEnergyAux
    variable = elastic_energy
  [../]
  [./vonmises]
    type = MaterialTensorAux
    tensor = stress
    variable = vonmises
    quantity = vonmises
  [../]
  [./hydrostatic]
    type = MaterialTensorAux
    tensor = stress
    variable = hydrostatic
    quantity = hydrostatic
  [../]
  [./fi]
    type = MaterialTensorAux
    tensor = stress
    variable = firstinv
    quantity = firstinvariant
  [../]
  [./si]
    type = MaterialTensorAux
    tensor = stress
    variable = secondinv
    quantity = secondinvariant
  [../]
  [./ti]
    type = MaterialTensorAux
    tensor = stress
    variable = thirdinv
    quantity = thirdinvariant
  [../]
  [./kinetic_energy]
    type = KineticEnergyAux
    variable = kinetic_energy
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]
[] # AuxKernels
