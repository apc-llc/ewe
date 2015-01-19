[Mesh]
      dim           = 3
      distribution  = DEFAULT
      nx            = 30
      ny            = 3
      nz            = 3
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
  [./dispx]  order=FIRST  family=LAGRANGE  [../]
  [./dispy]  order=FIRST  family=LAGRANGE  [../]
  [./dispz]  order=FIRST  family=LAGRANGE  [../]

  [./pressure]  order=CONSTANT  family=MONOMIAL  [../]
[]

[Kernels]
  [./stressdiv_dispx]  type=CardiacKirchhoffStressDivergence  variable=dispx  component=0  displacements='dispx dispy dispz'  [../]
  [./stressdiv_dispy]  type=CardiacKirchhoffStressDivergence  variable=dispy  component=1  displacements='dispx dispy dispz'  [../]
  [./stressdiv_dispz]  type=CardiacKirchhoffStressDivergence  variable=dispz  component=2  displacements='dispx dispy dispz'  [../]
  
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
  [../]

[]


[BCs]
  [./displacement_x]  type=PresetBC  variable=dispx  boundary=left  value=0.0  [../]
  [./displacement_y]  type=PresetBC  variable=dispy  boundary=left  value=0.0  [../]
  [./displacement_z]  type=PresetBC  variable=dispz  boundary=left  value=0.0  [../]

  [./no_pressure_xr]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary=right  value=0.0  [../]
  [./no_pressure_xt]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary=top    value=0.0  [../]
  [./no_pressure_xf]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary=front  value=0.0  [../]
  [./no_pressure_xb]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary=back   value=0.0  [../]

  [./no_pressure_yr]  type=CardiacMechanicsPressureBC  variable=dispy  component=0  boundary=right  value=0.0  [../]
  [./no_pressure_yt]  type=CardiacMechanicsPressureBC  variable=dispy  component=0  boundary=top    value=0.0  [../]
  [./no_pressure_yf]  type=CardiacMechanicsPressureBC  variable=dispy  component=0  boundary=front  value=0.0  [../]
  [./no_pressure_yb]  type=CardiacMechanicsPressureBC  variable=dispy  component=0  boundary=back   value=0.0  [../]

  [./no_pressure_zr]  type=CardiacMechanicsPressureBC  variable=dispz  component=0  boundary=right  value=0.0  [../]
  [./no_pressure_zt]  type=CardiacMechanicsPressureBC  variable=dispz  component=0  boundary=top    value=0.0  [../]
  [./no_pressure_zf]  type=CardiacMechanicsPressureBC  variable=dispz  component=0  boundary=front  value=0.0  [../]
  [./no_pressure_zb]  type=CardiacMechanicsPressureBC  variable=dispz  component=0  boundary=back   value=0.0  [../]

  [./pressure_x]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary='bottom'  value=100.004  [../]
  [./pressure_y]  type=CardiacMechanicsPressureBC  variable=dispy  component=1  boundary='bottom'  value=100.004  [../]
  [./pressure_z]  type=CardiacMechanicsPressureBC  variable=dispz  component=2  boundary='bottom'  value=100.004  [../]
[]

[Postprocessors]
  [./volume_ratio]  type=CardiacMaterialVolumeRatioPostprocessor  [../]
  [./timestep]      type=TimestepSize                             execute_on='timestep_end'  [../]
[]

[Executioner]
  type = Steady

  solve_type = PJFNK
  petsc_options_iname='-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value=' 201                hypre    boomeramg      4'
  petsc_options='-fp_trap -info -snes_monitor -snes_converged_reason -ksp_monitor -ksp_converged_reason  -ksp_monitor_true_residual -pc_svd_monitor'
  #line_search = 'cubic'

  nl_rel_step_tol = 1.e-8
  #l_max_its = 10
[]

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
