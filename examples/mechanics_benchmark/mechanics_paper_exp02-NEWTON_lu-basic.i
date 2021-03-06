[Mesh]
      file = 'ventricle_coarse.e'
      displacements = 'dispx dispy dispz'
      block_id='1'
      block_name='all'
[]

[Variables]
  [./dispx]  order=SECOND  family=LAGRANGE  [../]
  [./dispy]  order=SECOND  family=LAGRANGE  [../]
  [./dispz]  order=SECOND  family=LAGRANGE  [../]

  [./pressure]  order=FIRST  family=LAGRANGE  [../]
[]

[Kernels]
  [./stressdiv_dispx]  type=CardiacKirchhoffStressDivergence  variable=dispx  component=0  displacements='dispx dispy dispz'  p=pressure  [../]
  [./stressdiv_dispy]  type=CardiacKirchhoffStressDivergence  variable=dispy  component=1  displacements='dispx dispy dispz'  p=pressure  [../]
  [./stressdiv_dispz]  type=CardiacKirchhoffStressDivergence  variable=dispz  component=2  displacements='dispx dispy dispz'  p=pressure  [../]
  
  [./incompressibility]  type=CardiacKirchhoffIncompressibilityLagrangeMultiplier  variable=pressure  displacements='dispx dispy dispz'  [../]
[]

[AuxVariables]
  [./stress_xx]  order=CONSTANT  family=MONOMIAL  [../]
  [./stress_yy]  order=CONSTANT  family=MONOMIAL  [../]
  [./stress_zz]  order=CONSTANT  family=MONOMIAL  [../]
  [./stress_xy]  order=CONSTANT  family=MONOMIAL  [../]
  [./stress_yz]  order=CONSTANT  family=MONOMIAL  [../]
  [./stress_xz]  order=CONSTANT  family=MONOMIAL  [../]
[]

[AuxKernels]
  [./aux_stress_xx]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=0  column=0  variable=stress_xx  [../]
  [./aux_stress_yy]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=1  column=1  variable=stress_yy  [../]
  [./aux_stress_zz]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=2  column=2  variable=stress_zz  [../]
  [./aux_stress_xy]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=0  column=1  variable=stress_xy  [../]
  [./aux_stress_yz]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=1  column=2  variable=stress_yz  [../]
  [./aux_stress_xz]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=0  column=2  variable=stress_xz  [../]
[]

[Materials]
  active = 'fibres cardiac_material_costa'

  [./fibres]
    type=CardiacFibresMaterial
    block = all
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
    material_parameters =           ' 10  1    1    1'
    maximumQ = 15
    displacements ='dispx dispy dispz'
    outputs=all
    hydrostatic_pressure=pressure
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
  [./timestep]      type=TimestepSize execute_on='timestep_end'  [../]
[]
 
[Executioner]
  type = Steady

  solve_type = NEWTON
  #splitting = saddlepoint_fieldsplit

  petsc_options_iname='-snes_type -ksp_type -pc_type -pc_factor_shift_type -snes_linesearch_type'
  petsc_options_value=' newtonls   preonly   lu       NONZERO               basic'
  petsc_options='-fp_trap
                 -info
                 -snes_converged_reason
                 -ksp_converged_reason
                 -ksp_monitor_true_residual
                 -snes_linesearch_monitor
                 -pc_svd_monitor
                 -snes_test_display
                 -snes_ksp_ew_conv
                 '
#                 -help
  #line_search = 'cubic'
  
  #nl_rel_tol = 1.e-12
  #l_max_its = 10
  nl_max_its = 1000
[]

[Preconditioning]
  active = my_smp
  [./my_smp]
    # since we are using solve_type=NEWTON instead of PJFNK, this block should be ignored but it isn't:
    # it still determines how the Jacobian looks like when evaluated by PETSc. The default is to only
    # include the diagonal block elements which will for sure be wrong if the Jacobian is directly
    # used for finding the Newton search direction instead of as a preconditioner as it is originally intended
    type = SMP
    #                                                        lower left Jacobian block    upper right Jacobian block
    #off_diag_row =    'dispx dispx dispy dispy dispz dispz   pressure pressure pressure   dispx    dispy    dispz'
    #off_diag_column = 'dispy dispz dispx dispz dispx dispy   dispx    dispy    dispz      pressure pressure pressure'
    full = true
    pc_side = left
  [../]
[]

#[Splits]
#  [./saddlepoint_fieldsplit]
#    splitting = 'disp pressure'
#    splitting_type  = schur
#    schur_type    = full
#    schur_pre     = S
#  [../]
#  [./disp]
#    vars = 'dispx dispy dispz'
#    petsc_options='-fp_trap
#                 -info
#                 -snes_converged_reason
#                 -ksp_converged_reason
#                 -ksp_monitor_true_residual
#                 -snes_linesearch_monitor
#                 '
#    petsc_options_iname = '-pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
#    petsc_options_value = '   hypre  boomeramg      8'
#  [../]
#  [./pressure]
#    vars = 'pressure'
#    petsc_options='-fp_trap
#                 -info
#                 -snes_converged_reason
#                 -ksp_converged_reason
#                 -ksp_monitor_true_residual
#                 -snes_linesearch_monitor
#                 '
#    petsc_options_iname = '-pc_type'
#    petsc_options_value = '    none'
#  [../]
#[]


[Outputs]
 [./console]
    type=Console
    perf_log=false
    output_on = 'initial nonlinear linear timestep_begin timestep_end'
  [../]
  
  [./out]
     type=Exodus
     output_on = 'initial nonlinear linear timestep_end'
  [../]
[]
