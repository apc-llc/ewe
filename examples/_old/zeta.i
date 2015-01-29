# This is our mechanics convergence test case

[Mesh]
      dim           = 3
      distribution  = DEFAULT
      nx            = 5
      ny            = 5
      nz            = 5
      type          = GeneratedMesh
      xmax          =  0.5
      xmin          = -0.5
      ymax          =  0.5
      ymin          = -0.5
      zmax          =  0.0
      zmin          = -0.5
      displacements = 'dispx dispy dispz'
      elem_type = HEX20
  block_id='0'
  block_name='all'

[]

[Variables]
  [./dispx]  order=SECOND  family=LAGRANGE  [../]
  [./dispy]  order=SECOND  family=LAGRANGE  [../]
  [./dispz]  order=SECOND  family=LAGRANGE  [../]

  [./hydrostatic_pressure]  order=FIRST  family=LAGRANGE  [../]
[]

[Kernels]
  [./stressdiv_dispx]  type=CardiacKirchhoffStressDivergence  variable=dispx  component=0  displacements='dispx dispy dispz'  [../]
  [./stressdiv_dispy]  type=CardiacKirchhoffStressDivergence  variable=dispy  component=1  displacements='dispx dispy dispz'  [../]
  [./stressdiv_dispz]  type=CardiacKirchhoffStressDivergence  variable=dispz  component=2  displacements='dispx dispy dispz'  [../]
  
  [./incompressibility]  type=CardiacKirchhoffIncompressibilityLagrangeMultiplier  variable=hydrostatic_pressure  displacements='dispx dispy dispz'  [../]

  #[./inertia_x] type=SecondOrderImplicitEulerWithDensity  variable=dispx  density=0.0  lumping=false [../]
  #[./inertia_y] type=SecondOrderImplicitEulerWithDensity  variable=dispy  density=0.0  lumping=false [../]
  #[./inertia_z] type=SecondOrderImplicitEulerWithDensity  variable=dispz  density=0.0  lumping=false [../]
[]

[Materials]
  active='fibres cardiac_material_nash'

  [./fibres]
    type=CardiacFibresMaterial
    block=all
    fixed_R='0 0 1 1 0 0 0 1 0'
    outputs=all
    output_properties='E_fibre_x E_fibre_y E_fibre_z E_sheet_x E_sheet_y E_sheet_z E_normal_x E_normal_y E_normal_z'
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
    Ta_function=active_tension
    p=hydrostatic_pressure
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
    Ta_function=active_tension
    p=hydrostatic_pressure
  [../]
[]

[Functions]
  [./active_tension]
    # we are pacing at the following "early activation sites" which were hand-picked in Paraview
    # to closely match [Potse2006, Figure 1]:
    #   0.8 6.4 -3.1
    #  -1.4 6.4 -2.9
    #   2.2 8.9 -4.2
    type = ParsedFunction
    value = 'A*t*
               ( exp(-0.5*(pow(x-0.5,2)+pow(y-0.5,2)+pow(z+0.5,2))/pow(w,2)))'
    vars = '  A   w' # amplitude and spatial width of the Gaussians
    vals = '0.2 0.25'
  [../]
[]


[BCs]
  [./dispx_fixed] type=DirichletBC  variable=dispx  boundary='front'  value=0 [../]
  [./dispy_fixed] type=DirichletBC  variable=dispy  boundary='front'  value=0 [../]
  [./dispz_fixed] type=DirichletBC  variable=dispz  boundary='front'  value=0 [../]
[]

[Postprocessors]
  [./volume_ratio]  type=CardiacMaterialVolumeRatioPostprocessor  [../]
  [./timestep]      type=TimestepSize                             execute_on='timestep_end'  [../]
[]

[Executioner]
  type=Transient

  solve_type=NEWTON
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

  start_time=0
  end_time  =15.0
  dtmin     =0.25
  dtmax     =0.25
[]

# BUG: 'hydrostatic_pressure does not exist'
#[Dampers]
#  [./disp_damp]  type=ConstantDamper  variable='dispx dispy dispz'   damping=0.7 [../]
#  [./p_damp]     type=ConstantDamper  variable=hydrostatic_pressure  damping=0.7 [../]
#[]

#[Splits]
#  [./saddlepoint_fieldsplit]
#    splitting = 'disp pressure'
#    splitting_type  = schur
#    schur_type    = full
#    schur_pre     = S
#    petsc_options = '-dm_view'
#  [../]
#  [./disp]
#    vars = 'dispx dispy dispz'
#    petsc_options = '-dm_view'
#    petsc_options_iname = '-pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
#    petsc_options_value = '   hypre  boomeramg      8                          '
#  [../]
#  [./pressure]
#    vars = 'hydrostatic_pressure'
#    petsc_options = '-dm_view'
#    petsc_options_iname = '-pc_type'
#    petsc_options_value = '    none'
#  [../]
#[]

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
