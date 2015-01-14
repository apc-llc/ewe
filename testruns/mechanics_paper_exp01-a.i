[Mesh]
      dim           = 3
      distribution  = DEFAULT
      nx            = 40
      ny            = 4
      nz            = 4
      type          = GeneratedMesh
      xmax          =  0.0
      xmin          =  1.0
      ymax          =  0.0
      ymin          =  0.1
      zmax          =  0.0
      zmin          =  0.1
      displacements = 'dispx dispy dispz'
      #elem_type = HEX20
      block_id='0'
      block_name='all'
[]

[Variables]
  [./dispx]  order=FIRST  family=LAGRANGE  [../]
  [./dispy]  order=FIRST  family=LAGRANGE  [../]
  [./dispz]  order=FIRST  family=LAGRANGE  [../]

  [./hydrostatic_pressure]  order=CONSTANT  family=MONOMIAL  [../]
[]

[Kernels]
  [./stressdiv_dispx]  type=CardiacKirchhoffStressDivergence  variable=dispx  component=0  displacements='dispx dispy dispz'  [../]
  [./stressdiv_dispy]  type=CardiacKirchhoffStressDivergence  variable=dispy  component=1  displacements='dispx dispy dispz'  [../]
  [./stressdiv_dispz]  type=CardiacKirchhoffStressDivergence  variable=dispz  component=2  displacements='dispx dispy dispz'  [../]
  
  [./incompressibility]  type=CardiacKirchhoffIncompressibilityLagrangeMultiplier  variable=hydrostatic_pressure  displacements='dispx dispy dispz'  [../]
[]

[Materials]
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
    p=hydrostatic_pressure
  [../]
[]


[BCs]
  [./dispx_fixed] type=DirichletBC  variable=dispx  boundary='left'  value=0 [../]
  [./dispy_fixed] type=DirichletBC  variable=dispy  boundary='left'  value=0 [../]
  [./dispz_fixed] type=DirichletBC  variable=dispz  boundary='left'  value=0 [../]
[]

[Postprocessors]
  [./volume_ratio]  type=CardiacMaterialVolumeRatioPostprocessor  [../]
  [./timestep]      type=TimestepSize                             execute_on='timestep_end'  [../]
[]

[Executioner]
  type=Steady

  solve_type=PJFNK
  #petsc_options_iname='-pc_type'
  #petsc_options_value=' lu      '
  petsc_options='-fp_trap -info -snes_converged_reason'

  nl_rel_tol=1e-5
  nl_abs_tol=1e-5
  nl_rel_step_tol=1e-6
  nl_abs_step_tol=1e-6

  l_tol=1.e-6
  l_max_its=30
  #l_abs_step_tol=1.e-12

  line_search=basic  # TODO: what else?
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
