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

[Materials]
  active = 'fibres cardiac_material_linear'

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
    p=pressure
  [../]

  [./cardiac_material_nash]
    type=CardiacHolzapfel2009Material
    block=all
    use_displaced_mesh=false
    # material parameters as given in Table 1 of [Holzapfel 2009]
    #in following order:     a,    b,   a_f,   b_f,  a_s,   b_s, a_fs,  b_fs
    material_parameters='0.059 8.023 18.472 16.026 2.481 11.120 0.216 11.436'
    displacements ='dispx dispy dispz'
    outputs=all
    output_properties='Kirchhoff_stress'
    p=pressure
  [../]
  
  [./cardiac_material_holzapfel]
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
    output_properties='Kirchhoff_stress'
    p=pressure
  [../]
  
  [./cardiac_material_linear]
    type = CardiacLinearMaterial
    block = all
    use_displaced_mesh = false
    # material parameters
    lambda = 1.0
    mu     = 1.0
    displacements ='dispx dispy dispz'
    outputs=all
    output_properties='Kirchhoff_stress'
    p=pressure
  [../]
[]


[BCs]
  [./dispx_fixed] type=DirichletBC  variable=dispx  boundary='left'  value=0 [../]
  [./dispy_fixed] type=DirichletBC  variable=dispy  boundary='left'  value=0 [../]
  [./dispz_fixed] type=DirichletBC  variable=dispz  boundary='left'  value=0 [../]
  
  [./dispz_fixed_right] type=DirichletBC  variable=dispz  boundary='right'  value=0.2 [../]
  #[./pressure_bc] type=DirichletBC  variable=pressure  boundary='front'  value='0.004' [../]
[]

[Postprocessors]
  [./volume_ratio]  type=CardiacMaterialVolumeRatioPostprocessor  [../]
  [./timestep]      type=TimestepSize                             execute_on='timestep_end'  [../]
[]

[Executioner]
  type=Steady

  solve_type=PJFNK
  #petsc_options_iname='-snes_type'
  #petsc_options_value=' newtontr'
  petsc_options='-fp_trap -info -snes_converged_reason -snes_view'
  
  splitting = saddlepoint_fieldsplit

  nl_rel_tol=1e-5
  nl_abs_tol=1e-5
  nl_rel_step_tol=1e-6
  nl_abs_step_tol=1e-6

  l_tol=1.e-6
  l_max_its=30
  #l_abs_step_tol=1.e-12

  #line_search=basic  # TODO: what else?
[]

[Preconditioning]
  [./fd_prec]
    type=FDP
    full=true
  [../]
[]

# BUG: 'hydrostatic_pressure does not exist'
#[Dampers]
#  [./disp_damp]  type=ConstantDamper  variable='dispx dispy dispz'   damping=0.7 [../]
#  [./p_damp]     type=ConstantDamper  variable=hydrostatic_pressure  damping=0.7 [../]
#[]

[Splits]
  [./saddlepoint_fieldsplit]
    splitting = 'disp pressure'
    splitting_type  = schur
    schur_type    = full
    schur_pre     = S
    petsc_options = '-dm_view'
  [../]
  [./disp]
    vars = 'dispx dispy dispz'
    petsc_options = '-dm_view'
    petsc_options_iname = '-pc_type'
    petsc_options_value = ' lu'
  [../]
  [./pressure]
    vars = 'pressure'
    petsc_options = '-dm_view'
    petsc_options_iname = '-pc_type'
    petsc_options_value = '    none'
  [../]
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
