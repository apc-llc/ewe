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
  [./dispx]  order=SECOND  family=LAGRANGE  [../]
  [./dispy]  order=SECOND  family=LAGRANGE  [../]
  [./dispz]  order=SECOND  family=LAGRANGE  [../]

  #[./pressure]  order=FIRST  family=LAGRANGE  [../]
[]

[Kernels]
  [./stressdiv_dispx]  type=CardiacKirchhoffStressDivergence  variable=dispx  component=0  displacements='dispx dispy dispz'  [../]
  [./stressdiv_dispy]  type=CardiacKirchhoffStressDivergence  variable=dispy  component=1  displacements='dispx dispy dispz'  [../]
  [./stressdiv_dispz]  type=CardiacKirchhoffStressDivergence  variable=dispz  component=2  displacements='dispx dispy dispz'  [../]
  
  #[./incompressibility]  type=CardiacKirchhoffIncompressibilityLagrangeMultiplier  variable=pressure  displacements='dispx dispy dispz'  [../]
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
    output_properties='E_fibre_x E_fibre_y E_fibre_z E_sheet_x E_sheet_y E_sheet_z E_normal_x E_normal_y E_normal_z'
  [../]

  [./cardiac_material_costa]
    type = CardiacCostaMaterial
    block = all
    use_displaced_mesh = false
    # material parameters in the order C, b_f, b_t, b_fs (C in kPa, b unitless)
    material_parameters =           '  2  8    2    4'
    displacements ='dispx dispy dispz'
    outputs=all
    output_properties='Kirchhoff_stress'
    #p=pressure
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
    output_properties='Kirchhoff_stress'
  [../]

[]


[BCs]
  [./displacement_x]  type=PresetBC  variable=dispx  boundary=left  value=0.0  [../]
  [./displacement_y]  type=PresetBC  variable=dispy  boundary=left  value=0.0  [../]
  [./displacement_z]  type=PresetBC  variable=dispz  boundary=left  value=0.0  [../]

  [./pressure_x]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary='right top bottom front back'  value=0.0  [../]
  [./pressure_y]  type=CardiacMechanicsPressureBC  variable=dispy  component=1  boundary='      top bottom front back'  value=0.0  [../]
  [./pressure_z]  type=CardiacMechanicsPressureBC  variable=dispz  component=2  boundary='right top bottom front back'  value=0.0  [../]

  [./d_z]  type=PresetBC  variable=dispy  boundary=right  value=0.1  [../]

#  [./dispx_fixed] type=DirichletBC  variable=dispx  boundary='left'  value=0 [../]
#  [./dispy_fixed] type=DirichletBC  variable=dispy  boundary='left'  value=0 [../]
#  [./dispz_fixed] type=DirichletBC  variable=dispz  boundary='left'  value=0 [../]
  
#  [./no_pressure_x]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary='right top front back'  value=0.0  [../]
#  [./no_pressure_y]  type=CardiacMechanicsPressureBC  variable=dispy  component=1  boundary='right top front back'  value=0.0  [../]
#  [./no_pressure_z]  type=CardiacMechanicsPressureBC  variable=dispz  component=2  boundary='right top front back'  value=0.0  [../]

#  [./pressure_x]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary='bottom'  value=0.004  [../]
#  [./pressure_y]  type=CardiacMechanicsPressureBC  variable=dispy  component=1  boundary='bottom'  value=0.004  [../]
#  [./pressure_z]  type=CardiacMechanicsPressureBC  variable=dispz  component=2  boundary='bottom'  value=0.004  [../]
[]

[Postprocessors]
  [./volume_ratio]  type=CardiacMaterialVolumeRatioPostprocessor  [../]
  [./timestep]      type=TimestepSize                             execute_on='timestep_end'  [../]
[]

[Executioner]
  type = Steady

  solve_type = PJFNK
  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                 hypre    boomeramg      4'
  #line_search = 'cubic'

  nl_rel_step_tol = 1.e-8
  l_max_its = 100
[]

[Outputs]
 [./console]
    type=Console
    perf_log=true
    output_on = 'initial nonlinear linear timestep_begin timestep_end'
  [../]
  
  [./coupled]
     type=Exodus
     output_on = 'initial nonlinear timestep_begin timestep_end'
  [../]
[]

[Debug]
  show_actions                   = 0                           # Print out the actions being executed
  show_material_props            = 0                           # Print out the material properties supplied for each block, face, neighbor, ...
                                                               # and/or sideset
  show_parser                    = 0                           # Shows parser block extraction and debugging information
  show_top_residuals             = 15                          # The number of top residuals to print out (0 = no output)
  show_var_residual_norms        = 1                           # Print the residual norms of the individual solution variables at each ...
                                                               # nonlinear iteration
  show_var_residual              = 'dispx dispy dispz' # Variables for which residuals will be sent to the output file
[]
