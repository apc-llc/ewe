[Mesh]
      dim           = 3
      distribution  = DEFAULT
      nx            = 4
      ny            = 3
      nz            = 2
      type          = GeneratedMesh
      xmax          =  1.0
      xmin          =  0.0
      ymax          =  0.1
      ymin          =  0.0
      zmax          =  0.1
      zmin          =  0.0
      displacements = 'dispx dispy dispz'
      elem_type = HEX20
      block_id='0'
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
  [./stress_xx]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=0  column=0  variable=stress_xx  [../]
  [./stress_yy]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=1  column=1  variable=stress_xx  [../]
  [./stress_zz]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=2  column=2  variable=stress_xx  [../]
  [./stress_xy]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=0  column=1  variable=stress_xx  [../]
  [./stress_yz]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=1  column=2  variable=stress_xx  [../]
  [./stress_xz]  type=MaterialSymmTensorAux  property=Kirchhoff_stress  row=0  column=2  variable=stress_xx  [../]
[]

[Materials]
  active = 'fibres cardiac_material_costa'

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
  
  [./cardiac_material_linear]
    type = CardiacLinearMaterial
    block = 0
    use_displaced_mesh = false
    # material parameters
    E  = 0.01e6
    nu = 0.3
    displacements ='dispx dispy dispz'
    outputs=all
    hydrostatic_pressure=pressure
  [../]

  [./cardiac_material_holzapfel]
    type=CardiacHolzapfel2009Material
    block=all
    use_displaced_mesh=false
    # material parameters as given in Table 1 of [Holzapfel 2009]
    #in following order:     a,    b,   a_f,   b_f,  a_s,   b_s, a_fs,  b_fs
    material_parameters='0.059 8.023 18.472 16.026 2.481 11.120 0.216 11.436'
    displacements ='dispx dispy dispz'
    outputs=all
    hydrostatic_pressure=pressure
  [../]
  
  [./cardiac_material_nash]
    type = CardiacNash2000Material
    block = all
    use_displaced_mesh = false
    # material parameters in the order 11 22 33 12 23 31 (symmetric)
    # taken from [Nash & Hunter, 2000], Table I
    k_MN = '1.937 0.028 0.310 1.000 1.000 1.000'
    a_MN = '0.523 0.681 1.037 0.731 0.886 0.731'
    b_MN = '1.351 5.991 0.398 2.000 2.000 2.000'
    displacements ='dispx dispy dispz'
    outputs=all
    hydrostatic_pressure=pressure
  [../]
[]


[BCs]
  [./displacement_x]  type=PresetBC  variable=dispx  boundary=left  value=0.0  [../]
  [./displacement_y]  type=PresetBC  variable=dispy  boundary=left  value=0.0  [../]
  [./displacement_z]  type=PresetBC  variable=dispz  boundary=left  value=0.0  [../]

  [./pressure_x]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary='bottom'  value=0.004  use_current_configuration=true  [../]
  [./pressure_y]  type=CardiacMechanicsPressureBC  variable=dispy  component=1  boundary='bottom'  value=0.004  use_current_configuration=true  [../]
  [./pressure_z]  type=CardiacMechanicsPressureBC  variable=dispz  component=2  boundary='bottom'  value=0.004  use_current_configuration=true  [../]
[]

[Postprocessors]
  [./volume_ratio]  type=CardiacMaterialVolumeRatioPostprocessor  [../]
  [./timestep]      type=TimestepSize execute_on='timestep_end'  [../]
[]

[Executioner]
  type = Steady

  solve_type = NEWTON
  #splitting = saddlepoint_fieldsplit

  petsc_options_iname='-snes_type -ksp_type -pc_type -pc_factor_shift_type '
  petsc_options_value=' newtontr   preonly   lu       NONZERO'
  petsc_options='-fp_trap
                 -info
                 -snes_converged_reason
                 -ksp_converged_reason
                 -ksp_monitor_true_residual
                 -snes_linesearch_monitor
                 -pc_svd_monitor
                 -snes_test_display
                 '
#                 -help
  #line_search = 'cubic'
  
  #nl_rel_tol = 1.e-12
  #l_max_its = 10
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
  
  [./coupled]
     type=Exodus
     output_on = 'initial nonlinear timestep_end'
  [../]
[]

[Debug]
  show_actions                   = 0                           # Print out the actions being executed
  show_material_props            = 0                           # Print out the material properties supplied for each block, face, neighbor, ...
                                                               # and/or sideset
  show_parser                    = 0                           # Shows parser block extraction and debugging information
  #show_top_residuals             = 15                          # The number of top residuals to print out (0 = no output)
  show_var_residual_norms        = 1                           # Print the residual norms of the individual solution variables at each ...
                                                               # nonlinear iteration
  show_var_residual              = 'dispx dispy dispz' # Variables for which residuals will be sent to the output file
[]
