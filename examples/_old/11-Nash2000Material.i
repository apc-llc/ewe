# use with moose/modules/solid_mechanics/solid_mechanics-opt

[Mesh]
  file = 07-heart_geometry_new.e
  displacements = 'dispx dispy dispz'
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

  #[./pressure]
  #  order = FIRST
  #  family = SCALAR
  #[../]
[]

[Kernels]
  #[./incompressibility]
  #  type = PressureLagrangeMultiplier
  #  variable = pressure
  #[../]

  [./stressdiv_x]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispx
    component = 0
    displacements ='dispx dispy dispz'
  [../]
#  [./inertia_x]
#    type     = SecondDerivativeNewmark
#    variable = dispx
#    density  = 0.0001
#    lumping  = true
#  [../]

  [./stressdiv_y]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispy
    component = 1
    displacements ='dispx dispy dispz'
  [../]
#  [./inertia_y]
#    type     = SecondDerivativeNewmark
#    variable = dispy
#    density  = 0.0001
#    lumping  = true
#  [../]

  [./stressdiv_z]
    type      = CardiacKirchhoffStressDivergence
    use_displaced_mesh = false
    variable  = dispz
    component = 2
    displacements ='dispx dispy dispz'
  [../]
#  [./inertia_z]
#    type     = SecondDerivativeNewmark
#    variable = dispz
#    density  = 0.0001
#    lumping  = true
#  [../]
[]


[BCs]
#  [./ring_x]
#    type = DirichletBC
#    variable = dispx
#    boundary = ns_LV_opening
#    value    = 0.
#  [../]
#  [./ring_y]
#    type = DirichletBC
#    variable = dispy
#    boundary = ns_LV_opening
#    value    = 0.
#  [../]
#  [./ring_z]
#    type = DirichletBC
#    variable = dispz
#    boundary = ns_LV_opening
#    value    = 0.
#  [../]

#  [./Pressure_LV_x]
#    type = Pressure
#    boundary  = ss_LV_inner
#    variable  = dispx
#    component = 0
#    factor    = 1.0
#    function  = pressure_time
#  [../]
#  [./Pressure_LV_y]
#    type = Pressure
#    boundary  = ss_LV_inner
#    variable  = dispy
#    component = 1
#    factor    = 1.0
#    function  = pressure_time
#  [../]
#  [./Pressure_LV_z]
#    type = Pressure
#    boundary  = ss_LV_inner
#    variable  = dispz
#    component = 2
#    factor    = 1.0
#    function  = pressure_time
#  [../]
[]

[Materials]
  [./cardiac_properties]
    type = CardiacFibresMaterial
    fixed_R = '1 0 0 0 1 0 0 0 1'
    block = 1
    outputs = all
  [../]

  [./linear_orthotropic]
    type = CardiacNash2000Material
    block = 1
    use_displaced_mesh = false
    # material parameters in the order 11 22 33 12 23 31 (symmetric), taken from [Nash & Hunter, 2000], Table I
    k_MN = '1.937 0.028 0.310 1.000 1.000 1.000'
    a_MN = '0.523 0.681 1.037 0.731 0.886 0.731'
    b_MN = '1.351 5.991 0.398 2.000 2.000 2.000'
    displacements ='dispx dispy dispz'
    #TODO: Ta = Ta                          # .. active strain, i.e. strain that results from electrophysiology, default (if unset) = 0
    Ta_function = active_tension_function
    #TODO: p = pressure                     # .. p (pressure / Lagrange multiplier for incompressibility)
  [../]

#   [./newmarkx]
#     type = NewmarkMaterial
#     block = 1
#     disp  = dispx
#   [../]
#   [./newmarky]
#     type = NewmarkMaterial
#     block = 1
#     disp  = dispy
#   [../]
#   [./newmarkz]
#     type = NewmarkMaterial
#     block = 1
#     disp  = dispz
#   [../]
[]

[Executioner]
  type = Transient

  solve_type = PJFNK
  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                 hypre    boomeramg      4'
  line_search = 'none'

  nl_rel_step_tol = 1.e-8
  l_max_its = 10

  start_time = 0
  end_time   = 3.0
  #num_steps = 10
  dtmax      = 0.1
  dtmin      = 0.1
[]

[Functions]
  [./active_tension_function]
    type = ParsedFunction
    value = '0.01*sin(2*pi*t/3.0)+1.'
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
##  [./elastic_energy]
##    order = CONSTANT
##    family = MONOMIAL
##  [../]
#  [./kinetic_energy]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
[] # AuxVariables

[AuxKernels]
##  [./elastic_energy]
##    type = ElasticEnergyAux
##    variable = elastic_energy
##  [../]
#  [./kinetic_energy]
#    type = KineticEnergyNewmarkAux
#    variable = kinetic_energy
#    displacements = 'dispx dispy dispz'
#  [../]
[] # AuxKernels
