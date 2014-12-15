[Mesh]
  file=07-heart_geometry_new.e
[]

[Variables]
  [./dispx] order=FIRST family=LAGRANGE [../]
  [./dispy] order=FIRST family=LAGRANGE [../]
  [./dispz] order=FIRST family=LAGRANGE [../]

  [./hydrostatic_pressure] order=FIRST family=SCALAR [../]
[]

[Kernels]
  [./stressdiv_dispx] type=CardiacKirchhoffStressDivergence  variable=dispx  component=0  displacements='dispx dispy dispz' [../]
  [./stressdiv_dispy] type=CardiacKirchhoffStressDivergence  variable=dispy  component=1  displacements='dispx dispy dispz' [../]
  [./stressdiv_dispz] type=CardiacKirchhoffStressDivergence  variable=dispz  component=2  displacements='dispx dispy dispz' [../]

  #[./inertia_x] type=SecondOrderImplicitEulerWithDensity  variable=dispx  density=0.0  lumping=false [../]
  #[./inertia_y] type=SecondOrderImplicitEulerWithDensity  variable=dispy  density=0.0  lumping=false [../]
  #[./inertia_z] type=SecondOrderImplicitEulerWithDensity  variable=dispz  density=0.0  lumping=false [../]
[]

[ScalarKernels]
  [./incompressibility] type=CardiacIncompressibilityLagrangeMultiplier  variable=hydrostatic_pressure  volume_ratio_postprocessor=volume_ratio [../]
[]

[AuxVariables]
  [./distance_outer]    order=FIRST family=LAGRANGE [../]
  [./distance_RV_inner] order=FIRST family=LAGRANGE [../]
  [./distance_LV_inner] order=FIRST family=LAGRANGE [../]
  [./thickness_parameter] order=FIRST family=LAGRANGE [../]
[]

[AuxKernels]
  [./auxdistance_outer]    type=VolumeNearestNodeDistanceAux  variable=distance_outer     block=1  paired_boundary=ss_outer    [../]
  [./auxdistance_RV_inner] type=VolumeNearestNodeDistanceAux  variable=distance_RV_inner  block=1  paired_boundary=ss_RV_inner [../]
  [./auxdistance_LV_inner] type=VolumeNearestNodeDistanceAux  variable=distance_LV_inner  block=1  paired_boundary=ss_LV_inner [../]

  [./auxthickness]
    type=CardiacThicknessParameterAux variable=thickness_parameter
    distance_RV_inner=distance_RV_inner
    distance_LV_inner=distance_LV_inner
    distance_outer   =distance_outer
  [../]
[]

[Materials]
  [./fibres]
    type=CardiacFibresMaterial
    block=1
    thickness_parameter=thickness_parameter
    #fixed_R='0.36 0.48 -0.8 -0.8 0.6 0.0 0.48 0.64 0.60'
    #fixed_R='0 0 1 1 0 0 0 1 0'
    outputs=all
  [../]

  [./cardiac_material]
    type=CardiacHolzapfel2009Material
    block=1
    use_displaced_mesh=false
    # material parameters as given in Table 1 of [Holzapfel 2009] in following order: a, b, a_f, b_f, a_s, b_s, a_fs, b_fs
    material_parameters='0.059 8.023 18.472 16.026 2.481 11.120 0.216 11.436'
    displacements ='dispx dispy dispz'
    outputs=all
    output_properties='Kirchhoff_stress'
    Ta_function=active_tension
    p=hydrostatic_pressure
  [../]
[]

[Functions]
  [./active_tension]
    type=PiecewiseLinear
    x='0.0  1.0'
    y='0.0  1.0'
    scale_factor=1
  [../]
[]

[BCs]
  [./dispx_fixed] type=PresetBC  variable=dispx  boundary='ss_LV_inner ss_RV_inner ss_outer'  value=0 [../]
  [./dispy_fixed] type=PresetBC  variable=dispy  boundary='ss_LV_inner ss_RV_inner ss_outer'  value=0 [../]
  [./dispz_fixed] type=PresetBC  variable=dispz  boundary='ss_LV_inner ss_RV_inner ss_outer'  value=0 [../]

#  [./ns_lower_polar_point_x] type=DirichletBC  variable=dispx  boundary=ns_lower_polar_point  value=0 [../]
#  [./ns_lower_polar_point_y] type=DirichletBC  variable=dispy  boundary=ns_lower_polar_point  value=0 [../]
#  [./ns_lower_polar_point_z] type=DirichletBC  variable=dispz  boundary=ns_lower_polar_point  value=0 [../]
#  [./ns_lower_polar_neighbour_x] type=DirichletBC  variable=dispx  boundary=ns_lower_polar_neighbour  value=0 [../]
#  [./ns_lower_polar_neighbour_y] type=DirichletBC  variable=dispy  boundary=ns_lower_polar_neighbour  value=0 [../]
#  [./ns_lower_polar_neighbour_z] type=DirichletBC  variable=dispz  boundary=ns_lower_polar_neighbour  value=0 [../]
#  [./ring_x] type=DirichletBC  variable=dispx  value=0  boundary=ns_LV_opening [../]
#  [./ring_y] type=DirichletBC  variable=dispy  value=0  boundary=ns_LV_opening [../]
#  [./ring_z] type=DirichletBC  variable=dispz  value=0  boundary=ns_LV_opening [../]
[]

[Postprocessors]
  #[./elastic_energy] type=ElementIntegralMaterialProperty execute_on=timestep mat_prop=elastic_energy_density [../]
  [./volume_ratio] type=CardiacMaterialVolumeRatioPostprocessor execute_on=residual [../]
[]

[Executioner]
  type=Transient

  solve_type=PJFNK
  petsc_options_iname='-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value=' 201                hypre    boomeramg      4                          '
  petsc_options='-fp_trap -info -snes_monitor -snes_converged_reason -ksp_monitor -ksp_converged_reason  -ksp_monitor_true_residual -pc_svd_monitor'

  nl_rel_tol=1e-3
  nl_abs_tol=1e-8
  nl_rel_step_tol=1e-8
  l_tol=1.e-8
  #l_max_its=20

  start_time=0
  end_time  =1.0
  #num_steps=10
  dtmax     =0.1
  dtmin     =0.1
[]

[Outputs]
  exodus=true

  [./console]
    type=Console
    perf_log=false
    linear_residuals=true
  [../]
[]

#[Debug]
#  show_actions                   = 0                           # Print out the actions being executed
#  show_material_props            = 1                           # Print out the material properties supplied for each block, face, neighbor, ...
#                                                               # and/or sideset
#  show_parser                    = 0                           # Shows parser block extraction and debugging information
#  show_top_residuals             = 15                          # The number of top residuals to print out (0 = no output)
#  show_var_residual_norms        = 1                           # Print the residual norms of the individual solution variables at each ...
#                                                               # nonlinear iteration
#  #show_var_residual              = 'dispx dispy dispz hydrostatic_pressure' # Variables for which residuals will be sent to the output file
#[]