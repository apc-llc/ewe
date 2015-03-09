[Mesh]
  block_id='0'
  block_name='all'
  displacements='dispx dispy dispz'
  dim           = 3
  distribution  = DEFAULT
  nx            = 32
  ny            = 8
  nz            = 8
  type          = GeneratedMesh
  xmax          = 10.0
  xmin          = 0.0
  ymax          = 2.5
  ymin          = 0.0
  zmax          = 2.5
  zmin          = 0.0
  elem_type     = HEX20
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

  [./cardiac_material_costa]
    type = CardiacCostaMaterial
    block = all
    use_displaced_mesh = false
    # material parameters in the order C, b_f, b_t, b_fs (C in kPa, b unitless)
    material_parameters =           '  2  8    2    4'
    maximumQ = 15
    displacements ='dispx dispy dispz'
    outputs=all
    hydrostatic_pressure=pressure
  [../]

  [./active_tension_material]
    type=ActiveTensionODE
    Vmem=potential_from_sub
    block=all
    # these are the default parameter values, including them here to make sure they are not forgotten as tunable options
    epsilon_recovery=0.01
    epsilon_development=0.04
    kTa=2.
    Vrest=-90.272
    Vmax=120.
  [../]
[]

[BCs]
 [./displacement_x]  type=PresetBC  variable=dispx  boundary=left  value=0.0  [../]
 [./displacement_y]  type=PresetBC  variable=dispy  boundary=left  value=0.0  [../]
 [./displacement_z]  type=PresetBC  variable=dispz  boundary=left  value=0.0  [../]
[]

[Postprocessors]
  [./volume_ratio]  type=CardiacMaterialVolumeRatioPostprocessor  [../]
  [./timestep]      type=TimestepSize  execute_on='timestep_end'  [../]
[]

[Executioner]
  type=Transient

  solve_type=PJFNK
#  petsc_options_iname='-snes_type -ksp_type -pc_type -pc_factor_shift_type'
#  petsc_options_value=' newtonls   preonly   lu       NONZERO'
#petsc_options='-fp_trap -info -snes_monitor -snes_converged_reason -ksp_monitor -ksp_converged_reason  -ksp_monitor_true_residual -pc_svd_monitor'

  nl_rel_tol=1e-5
  nl_abs_tol=1e-5
  nl_rel_step_tol=1e-6
  nl_abs_step_tol=1e-6

  l_tol=1.e-6
  l_max_its=30
  #l_abs_step_tol=1.e-12
  #l_max_its=20

  start_time=0
  end_time  =20.0
  dtmin     =0.25
  dtmax     =0.25
 []


[Outputs]
  [./console]
  type=Console
  perf_log=false
  output_on = 'initial timestep_begin timestep_end'
  [../]

  [./out]
  type=Exodus
  output_on = 'initial timestep_end'
  [../]
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
    type=MultiAppMeshFunctionTransfer
    direction=from_multiapp
    execute_on=timestep_begin
    multi_app=electrocardio
    source_variable=potential
    variable=potential_from_sub
  [../]
[]