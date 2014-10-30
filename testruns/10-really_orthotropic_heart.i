# use with moose/modules/solid_mechanics/solid_mechanics-opt

[Mesh]
  file = 07-heart_geometry_new.e
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

[Kernels]
  [./stressdiv_x]
    type      = CardiacStressDivergence
    variable  = disp_x
    component = 0
    disp_x    = disp_x
    disp_y    = disp_y
    disp_z    = disp_z
  [../]
  [./inertia_x]
    type     = SecondDerivativeNewmark
    variable = disp_x
    density  = 0.1
    lumping  = true
  [../]

  [./stressdiv_y]
    type      = CardiacStressDivergence
    variable  = disp_y
    component = 1
    disp_x    = disp_x
    disp_y    = disp_y
    disp_z    = disp_z
  [../]
  [./inertia_y]
    type     = SecondDerivativeNewmark
    variable = disp_y
    density  = 0.1
    lumping  = true
  [../]

  [./stressdiv_z]
    type      = CardiacStressDivergence
    variable  = disp_z
    component = 2
    disp_x    = disp_x
    disp_y    = disp_y
    disp_z    = disp_z
  [../]
  [./inertia_z]
    type     = SecondDerivativeNewmark
    variable = disp_z
    density  = 0.1
    lumping  = true
  [../]
[]


[BCs]
  [./ring_x]
    type = DirichletBC
    variable = disp_x
    boundary = ns_LV_opening
    value    = 0.
  [../]
  [./ring_y]
    type = DirichletBC
    variable = disp_y
    boundary = ns_LV_opening
    value    = 0.
  [../]
  [./ring_z]
    type = DirichletBC
    variable = disp_z
    boundary = ns_LV_opening
    value    = 0.
  [../]

  [./Pressure_LV_x]
    type = Pressure
    boundary  = ss_LV_inner
    variable  = disp_x
    component = 0
    factor    = 1.0
    function  = pressure_time
  [../]
  [./Pressure_LV_y]
    type = Pressure
    boundary  = ss_LV_inner
    variable  = disp_y
    component = 1
    factor    = 1.0
    function  = pressure_time
  [../]
  [./Pressure_LV_z]
    type = Pressure
    boundary  = ss_LV_inner
    variable  = disp_z
    component = 2
    factor    = 1.0
    function  = pressure_time
  [../]
[]

[Materials]
  [./cardiac_properties]
   type = CardiacFibresMaterial
   block = 1
   outputs = all
  [../]

  [./linear_orthotropic]
    type = CardiacLinearOrthotropicMaterial
    block = 1
    youngs_moduli   = '1.e5 1.e4 1.e4'
    poissons_ratios = '0.3 0.3 0.3'
    # linear isotropic: shear_modulus = youngs_modulus / (2 * (1+poissons_ratio) )
    shear_moduli    = '38461.538462 3846.1538462 3846.1538462'
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]

   [./newmarkx]
     type = NewmarkMaterial
     block = 1
     disp  = disp_x
   [../]
   [./newmarky]
     type = NewmarkMaterial
     block = 1
     disp  = disp_y
   [../]
   [./newmarkz]
     type = NewmarkMaterial
     block = 1
     disp  = disp_z
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
  [./pressure_time]
    type = ParsedFunction
    value = '2500.*(sin(2*pi*t/3.0)+1.)'
  [../]
[]

[Outputs]
  exodus = true
  
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
  
  [./exodus_displaced]
     file_base = out
     type = Exodus
     use_displaced = true
  [../]  
[]


[AuxVariables]
#  [./temperature]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./stress_xx]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./stress_yy]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./stress_zz]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./stress_xy]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./stress_yz]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./stress_zx]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
  [./elastic_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
#  [./vonmises]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./hydrostatic]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./firstinv]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./secondinv]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./thirdinv]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
  [./kinetic_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[] # AuxVariables

[AuxKernels]
#  [./temperature_time]
#    type = FunctionAux
#    variable = temperature
#    function = temptime
#  [../]
#
#  [./stress_xx]
#    type = MaterialTensorAux
#    tensor = stress
#    variable = stress_xx
#    index = 0
#  [../]
#  [./stress_yy]
#    type = MaterialTensorAux
#    tensor = stress
#    variable = stress_yy
#    index = 1
#  [../]
#  [./stress_zz]
#    type = MaterialTensorAux
#    tensor = stress
#    variable = stress_zz
#    index = 2
#  [../]
#  [./stress_xy]
#    type = MaterialTensorAux
#    tensor = stress
#    variable = stress_xy
#    index = 3
#  [../]
#  [./stress_yz]
#    type = MaterialTensorAux
#    tensor = stress
#    variable = stress_yz
#    index = 4
#  [../]
#  [./stress_zx]
#    type = MaterialTensorAux
#    tensor = stress
#    variable = stress_zx
#    index = 5
#  [../]
  [./elastic_energy]
    type = ElasticEnergyAux
    variable = elastic_energy
  [../]
#  [./vonmises]
#    type = MaterialTensorAux
#    tensor = stress
#    variable = vonmises
#    quantity = vonmises
#  [../]
#  [./hydrostatic]
#    type = MaterialTensorAux
#    tensor = stress
#    variable = hydrostatic
#    quantity = hydrostatic
#  [../]
#  [./fi]
#    type = MaterialTensorAux
#    tensor = stress
#    variable = firstinv
#    quantity = firstinvariant
#  [../]
#  [./si]
#    type = MaterialTensorAux
#    tensor = stress
#    variable = secondinv
#    quantity = secondinvariant
#  [../]
#  [./ti]
#    type = MaterialTensorAux
#    tensor = stress
#    variable = thirdinv
#    quantity = thirdinvariant
#  [../]
  [./kinetic_energy]
    type = KineticEnergyNewmarkAux
    variable = kinetic_energy
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]
[] # AuxKernels
