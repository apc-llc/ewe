[Mesh]
 file = mug.e
 displacements = 'disp_x disp_y disp_z'
 []

[Variables]
  [./disp_x] order = FIRST family = LAGRANGE [../]
  [./disp_y] order = FIRST family = LAGRANGE [../]
  [./disp_z] order = FIRST family = LAGRANGE [../]
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
  [./inertia_x] type = SecondOrderImplicitEulerWithDensity variable = disp_x density = 0.1 lumping = false [../]
  [./inertia_y] type = SecondOrderImplicitEulerWithDensity variable = disp_y density = 0.1 lumping = false [../]
  [./inertia_z] type = SecondOrderImplicitEulerWithDensity variable = disp_z density = 0.1 lumping = false [../]
[]


[BCs]
  [./ring_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom'
    value    = 0.
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
    thermal_expansion = -0.004
    t_ref = 0
    temp = active_tension
  [../]
 
   [./active_tension_material]
     type = ActiveTensionODE
     Vmem = potential_from_sub
     block = 1
    # these are the default parameter values, including them here to make sure they are not forgotten as tunable options
     epsilon_recovery = 0.01
     epsilon_development = 0.04
     kTa = 47.9
     Vrest = -90.272
     Vmax = 0.
   [../]
 
[]

[Executioner]
  type = Transient

  solve_type = PJFNK
  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                 hypre    boomeramg      4'
#petsc_options = '-fp_trap -info
#     -snes_monitor -snes_converged_reason -snes_mf_operator
#     -ksp_monitor  -ksp_converged_reason  -ksp_monitor_true_residual
#     -pc_svd_monitor'
 
  line_search = 'none'

  nl_rel_step_tol = 1.e-5
  l_max_its = 100

  start_time = 0
  end_time   = 500.0
  dtmax      = 0.1
  dtmin      = 0.1
[]

[Outputs]
#  exodus = true
  
  [./console]
    type = Console
    perf_log = true
    linear_residuals = false
  [../]
  
  [./exodus_displaced]
#file_base = out
     type = Exodus
#     use_displaced = true
  [../]
[]


[AuxVariables]
  [./potential_from_sub] order = FIRST family = LAGRANGE [../]
  [./active_tension] order = CONSTANT family = MONOMIAL [../]

 []

[AuxKernels]
 [./aux_active_tension]  type = MaterialRealAux property = active_tension variable = active_tension [../]
[] # AuxKernels

[MultiApps]
  [./electrocardio]
  type = TransientMultiApp
  app_type = EweApp
  execute_on = timestep_begin
  input_files = alpha-Coupling_Electrocardio-Thermoelasticity_mug_sub.i
  positions = '0.0 0.0 0.0'
  #boundary = right
  [../]
[]

[Transfers]
#[./to_sub]
#  type = MultiAppNearestNodeTransfer
#  direction = to_multiapp
#  execute_on = timestep
#  multi_app = conv
#  source_variable = diffused
#  variable = from_master
#  fixed_meshes=true # independent of any deformation we want to make sure that transfer always happens between the same node pairs
#  [../]
  [./from_sub]
  type = MultiAppNearestNodeTransfer
  direction = from_multiapp
  execute_on = timestep
  multi_app = electrocardio
  source_variable = potential
  variable = potential_from_sub
  fixed_meshes=true # independent of any deformation we want to make sure that transfer always happens between the same node pairs
  [../]
[]
