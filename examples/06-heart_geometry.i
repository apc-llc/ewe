# use with moose/modules/solid_mechanics/solid_mechanics-opt

[Mesh]
  file = 06-heart_geometry.e
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
    type     = SecondOrderImplicitEulerWithDensity
    variable = disp_x
    density  = 0.1
    lumping  = false
  [../]
  [./inertia_y]
    type     = SecondOrderImplicitEulerWithDensity
    variable = disp_y
    density  = 0.1
    lumping  = false
  [../]
  [./inertia_z]
    type     = SecondOrderImplicitEulerWithDensity
    variable = disp_z
    density  = 0.1
    lumping  = false
  [../]
[]


[BCs]
  [./force_x]
    type = NeumannBC
    variable = disp_x
    boundary = '1 2 3'
    value = 0.0
  [../]
  [./force_y]
    type = NeumannBC
    variable = disp_y
    boundary = '1 2 3'
    value = 0.0
  [../]
  [./force_z]
    type = NeumannBC
    variable = disp_z
    boundary = '1 2 3'
    value = 0.0
  [../]
[]

[Materials]
  [./linear_isotropic]
    type = LinearIsotropicMaterial
    block = 1
    youngs_modulus = 1.e5
    poissons_ratio = 0.3
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
    # thermal properties
    thermal_expansion = 1.0
    t_ref = 0.0
    temp = temperature
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
  dtmin      = 0.1
[]

[Functions]
  [./temptime]
    type = ParsedFunction
    value = '1.0*sin(2*pi*t/3.0)*exp(-(x*x+y*y+z*z))'
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
  [./temperature]
    order = CONSTANT
    family = MONOMIAL
  [../]
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
[] # AuxVariables

[AuxKernels]
  [./temperature_time]
    type = FunctionAux
    variable = temperature
    function = temptime
  [../]

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
[] # AuxKernels
