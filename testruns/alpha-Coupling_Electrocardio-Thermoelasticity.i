[Mesh]
#file = 07-heart_geometry_new.e
#  displacements = 'disp_x disp_y disp_z'
  dim           = 3
  distribution  = DEFAULT
  nx            = 5
  ny            = 5
  nz            = 5
  type          = GeneratedMesh
  xmax          = 5.0
  xmin          = 0.0
  ymax          = 5.0
  ymin          = 0.0
  zmax          = 5.0
  zmin          = 0.0
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
    boundary = 'left'
    value    = 0.
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
    # thermal properties
    thermal_expansion = -1
    t_ref = 0
    temp = active_tension_from_sub
  [../]
[]

[Executioner]
  type = Transient

  solve_type = PJFNK
# petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
# petsc_options_value = '201                 hypre    boomeramg      4'
  line_search = 'none'

  nl_rel_step_tol = 1.e-8
  l_max_its = 100

  start_time = 0
  end_time   = 500.0
  dtmax      = 0.05
  dtmin      = 0.05
[]

[Outputs]
  exodus = true
  
  [./console]
    type = Console
    perf_log = true
    linear_residuals = false
  [../]
  
#  [./exodus_displaced]
#     file_base = out
#     type = Exodus
#     #use_displaced = true
#  [../]  
[]


[AuxVariables]
  [./active_tension_from_sub] order = CONSTANT family = MONOMIAL [../]
[]

[AuxKernels]
[] # AuxKernels

[MultiApps]
  [./electrocardio]
  type = TransientMultiApp
  app_type = EweApp
  execute_on = timestep_begin
  input_files = alpha-Coupling_Electrocardio-Thermoelasticity_sub.i
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
  type = MultiAppInterpolationTransfer # MultiAppNearestNodeTransfer does not seem to work for elemental (type=CONSTANT) AuxVariables...
  direction = from_multiapp
  execute_on = timestep
  multi_app = electrocardio
  source_variable = active_tension
  variable = active_tension_from_sub
  fixed_meshes=true # independent of any deformation we want to make sure that transfer always happens between the same node pairs
  [../]
[]