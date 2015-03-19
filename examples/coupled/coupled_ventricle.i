[Mesh]
  file = 'ventricle_fine.e'
  block_id = '1'
  block_name = 'all'
  uniform_refine = 0
  displacements='dispx dispy dispz'
[]

[Variables]
  [./dispx]     order=SECOND  family=LAGRANGE  [../]
  [./dispy]     order=SECOND  family=LAGRANGE  [../]
  [./dispz]     order=SECOND  family=LAGRANGE  [../]
  [./pressure]  order=FIRST  family=LAGRANGE  [../]
 []

[Kernels]
  [./stressdiv_dispx]  type=CardiacKirchhoffStressDivergence  variable=dispx  component=0  displacements='dispx dispy dispz'  p=pressure [../]
  [./stressdiv_dispy]  type=CardiacKirchhoffStressDivergence  variable=dispy  component=1  displacements='dispx dispy dispz'  p=pressure [../]
  [./stressdiv_dispz]  type=CardiacKirchhoffStressDivergence  variable=dispz  component=2  displacements='dispx dispy dispz'  p=pressure [../]
  [./incompressibility]  type=CardiacKirchhoffIncompressibilityLagrangeMultiplier  variable=pressure  displacements='dispx dispy dispz'  [../]
[]


[AuxVariables]
  [./stress_xx]  order=CONSTANT  family=MONOMIAL  [../]
  [./stress_yy]  order=CONSTANT  family=MONOMIAL  [../]
  [./stress_zz]  order=CONSTANT  family=MONOMIAL  [../]
  [./stress_xy]  order=CONSTANT  family=MONOMIAL  [../]
  [./stress_yz]  order=CONSTANT  family=MONOMIAL  [../]
  [./stress_xz]  order=CONSTANT  family=MONOMIAL  [../]
  [./active_tension]  order=CONSTANT  family=MONOMIAL  [../]
  [./potential_from_sub]  order=FIRST family=LAGRANGE [../]
[]

[AuxKernels]
 [./aux_stress_xx]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=0  column=0  variable=stress_xx  [../]
 [./aux_stress_yy]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=1  column=1  variable=stress_yy  [../]
 [./aux_stress_zz]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=2  column=2  variable=stress_zz  [../]
 [./aux_stress_xy]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=0  column=1  variable=stress_xy  [../]
 [./aux_stress_yz]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=1  column=2  variable=stress_yz  [../]
 [./aux_stress_xz]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=0  column=2  variable=stress_xz  [../]
 [./aux_active_tension]  type=MaterialRealAux  property=active_tension  variable=active_tension  [../]
[]

[Materials]
  [./fibres]
    type=CardiacFibresMaterial
    block=all
    fixed_R='1 0 0
    0 1 0
    0 0 1'
    outputs=all
  [../]

   [./cardiac_material_linear]
     type = CardiacLinearMaterial
     block = 1
     use_displaced_mesh = false
    # material parameters
     E  = 1.e2
     nu = 0.3
     displacements ='dispx dispy dispz'
     outputs=all
     active_tension = active_tension
   [../]


  [./active_tension_material]
    type=ActiveTensionODE
    Vmem=potential_from_sub
    block=all
    # these are the default parameter values, including them here to make sure they are not forgotten as tunable options
    epsilon_recovery=0.01
    epsilon_development=0.04
    kTa=47.9
    Vrest=-90.272
    Vmax=150.
  [../]
[]

[BCs]
 [./displacement_x]  type=PresetBC  variable=dispx  boundary=103  value=0.0  [../]
 [./displacement_y]  type=PresetBC  variable=dispy  boundary=103  value=0.0  [../]
 [./displacement_z]  type=PresetBC  variable=dispz  boundary=103  value=0.0  [../]
 
 [./pressure_x]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary=101  value=1.  use_current_configuration=true  displacements='dispx dispy dispz'  [../]
 [./pressure_y]  type=CardiacMechanicsPressureBC  variable=dispy  component=1  boundary=101  value=1.  use_current_configuration=true  displacements='dispx dispy dispz'  [../]
 [./pressure_z]  type=CardiacMechanicsPressureBC  variable=dispz  component=2  boundary=101  value=1.  use_current_configuration=true  displacements='dispx dispy dispz'  [../]
[]

[Postprocessors]
  [./volume_ratio]  type=CardiacMaterialVolumeRatioPostprocessor  [../]
  [./timestep]      type=TimestepSize  execute_on='timestep_end'  [../]
[]
 
[Executioner]
  type=Transient

  solve_type=PJFNK
  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                 hypre    boomeramg      4'
#  petsc_options='-snes_monitor -snes_converged_reason'
  line_search = 'none'
 
  nl_rel_tol      = 1e-5
  nl_abs_tol      = 1e-5
  nl_rel_step_tol = 1e-6
  nl_abs_step_tol = 1e-6

  l_tol           = 1.e-6
  l_max_its       = 15
  #l_abs_step_tol=1.e-12
  #l_max_its=20

  start_time=   0.0
  end_time  = 500.0
  dtmin     =   0.1
  dtmax     =   0.5
 []

[Outputs]
  [./console]
  type=Console
  perf_log=false
  output_on = 'initial nonlinear timestep_begin timestep_end'
  [../]

  exodus=true
  output_on = 'initial timestep_end'
[]

[MultiApps]
  [./electrocardio]
    type=TransientMultiApp
    app_type=EweApp
    execute_on=timestep_begin
    input_files=coupled_bar_sub.i
    positions='0.0 0.0 0.0'
  [../]
[]
#
[Transfers]
  [./dispx_to_sub]
    type=MultiAppMeshFunctionTransfer
    direction=to_multiapp
    execute_on=timestep_begin
    multi_app=electrocardio
    source_variable=dispx
    variable=dispx
  [../]
  [./dispy_to_sub]
    type=MultiAppMeshFunctionTransfer
    direction=to_multiapp
    execute_on=timestep_begin
    multi_app=electrocardio
    source_variable=dispy
    variable=dispy
  [../]
  [./dispz_to_sub]
    type=MultiAppMeshFunctionTransfer
    direction=to_multiapp
    execute_on=timestep_begin
    multi_app=electrocardio
    source_variable=dispz
    variable=dispz
  [../]
  [./from_sub]
#type=MultiAppNearestNodeTransfer # This, for some reason, leads to a segfault
    type=MultiAppMeshFunctionTransfer
    direction=from_multiapp
    execute_on=timestep_begin
    multi_app=electrocardio
    source_variable=potential
    variable=potential_from_sub
  [../]
[]