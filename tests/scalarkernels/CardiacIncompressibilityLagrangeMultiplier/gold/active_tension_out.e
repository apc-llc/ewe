CDF      
      
len_string     !   len_line   Q   four      	time_step          len_name   !   num_dim       	num_nodes         num_elem      
num_el_blk        num_node_sets         num_side_sets         num_el_in_blk1        num_nod_per_el1       num_side_ss1      num_side_ss2      num_side_ss3      num_side_ss4      num_side_ss5      num_side_ss6      num_nod_ns1       num_nod_ns2       num_nod_ns3       num_nod_ns4       num_nod_ns5       num_nod_ns6       num_nod_var       num_glo_var       num_info  �         api_version       @�
=   version       @�
=   floating_point_word_size            	file_size               int64_status             title         active_tension_out.e       maximum_name_length                 *   
time_whole                            ��   	eb_status                             0   eb_prop1               name      ID              4   	ns_status         	                    8   ns_prop1      	         name      ID              P   	ss_status         
                    h   ss_prop1      
         name      ID              �   coordx                      @      �   coordy                      @      �   coordz                      @         eb_names                       $      X   ns_names      	                 �      |   ss_names      
                 �      D   
coor_names                         d         node_num_map                           p   connect1                  	elem_type         HEX8                 �   elem_num_map                          �   elem_ss1                          �   side_ss1                          �   elem_ss2                          �   side_ss2                          �   elem_ss3                          �   side_ss3                          �   elem_ss4                          �   side_ss4                          �   elem_ss5                          �   side_ss5                          �   elem_ss6                          �   side_ss6                          �   node_ns1                          �   node_ns2                          �   node_ns3                             node_ns4                             node_ns5                          $   node_ns6                          4   vals_nod_var1                          @      ��   vals_nod_var2                          @      ��   vals_nod_var3                          @      �   name_nod_var                       d      D   name_glo_var                       D      �   vals_glo_var                             �H   info_records                      ��      �                                                                                         ?�      ?�                      ?�      ?�                              ?�      ?�                      ?�      ?�                                      ?�      ?�      ?�      ?�                                          back                             bottom                           left                             front                            top                              right                              back                             front                            bottom                           top                              left                             right                                                                                                                                                                                                                                                                                                 dispx                            dispy                            dispz                             volume_ratio        ������ ˿����hydrostatic_pressure ��?*Ӧ?���w�####################?�ь)� �                ����A�?�3��r��                �����# Created by MOOSE #       ?���?*Ӧ?���w�        ?�     ?Ć�,��              ####################                                                             ### Command Line Arguments ###                                                   -i                                                                               active_tension.i                                                                                                                                                  ### Version Info ###                                                             Framework Information:                                                           MOOSE version:           git commit 7b9526f on 2014-11-06                        PETSc Version:           3.5.1                                                   Current Time:            Thu Nov  6 17:49:43 2014                                Executable Timestamp:    Thu Nov  6 17:37:34 2014                                                                                                                                                                                                  ### Input File ###                                                                                                                                                []                                                                                 initial_from_file_timestep     = 2                                               initial_from_file_var          = INVALID                                         block                          = INVALID                                         coord_type                     = XYZ                                             fe_cache                       = 0                                               kernel_coverage_check          = 1                                               name                           = 'MOOSE Problem'                                 type                           = FEProblem                                       use_legacy_uo_aux_computation  = INVALID                                         use_legacy_uo_initialization   = INVALID                                         element_order                  = AUTO                                            order                          = AUTO                                            side_order                     = AUTO                                            active_bcs                     = INVALID                                         active_kernels                 = INVALID                                         inactive_bcs                   = INVALID                                         inactive_kernels               = INVALID                                         start                          = 0                                               dimNearNullSpace               = 0                                               dimNullSpace                   = 0                                               long_name                      =                                                 solve                          = 1                                               use_nonlinear                  = 1                                             []                                                                                                                                                                [BCs]                                                                                                                                                               [./bc_dispx]                                                                       boundary                     = left                                              type                         = DirichletBC                                       use_displaced_mesh           = 0                                                 variable                     = dispx                                             long_name                    = BCs/bc_dispx                                      seed                         = 0                                                 value                        = 0                                               [../]                                                                                                                                                             [./bc_dispz]                                                                       boundary                     = left                                              type                         = DirichletBC                                       use_displaced_mesh           = 0                                                 variable                     = dispz                                             long_name                    = BCs/bc_dispz                                      seed                         = 0                                                 value                        = 0                                               [../]                                                                                                                                                             [./bc_y]                                                                           boundary                     = left                                              type                         = DirichletBC                                       use_displaced_mesh           = 0                                                 variable                     = dispy                                             long_name                    = BCs/bc_y                                          seed                         = 0                                                 value                        = 0                                               [../]                                                                          []                                                                                                                                                                [Executioner]                                                                      type                           = Steady                                          l_abs_step_tol                 = -1                                              l_max_its                      = 100                                             l_tol                          = 1e-05                                           line_search                    = none                                            nl_abs_step_tol                = 1e-50                                           nl_abs_tol                     = 1e-50                                           nl_max_funcs                   = 10000                                           nl_max_its                     = 50                                              nl_rel_step_tol                = 1e-08                                           nl_rel_tol                     = 1e-08                                           no_fe_reinit                   = 0                                               petsc_options                  = INVALID                                         petsc_options_iname            = '-ksp_gmres_restart -pc_type -pc_hypre_typ... e -pc_hypre_boomeramg_max_iter'                                                    petsc_options_value            = '201 hypre boomeramg 4'                         solve_type                     = PJFNK                                           long_name                      = Executioner                                     restart_file_base              =                                                 splitting                      = INVALID                                       []                                                                                                                                                                [Executioner]                                                                      _fe_problem                    = 0x7fd93b03be00                                []                                                                                                                                                                [Functions]                                                                                                                                                         [./active_tension]                                                                 type                         = ConstantFunction                                  long_name                    = Functions/active_tension                          value                        = 1                                               [../]                                                                          []                                                                                                                                                                [Kernels]                                                                                                                                                           [./stressdiv_dispx]                                                                type                         = CardiacKirchhoffStressDivergence                  block                        = INVALID                                           component                    = 0                                                 diag_save_in                 = INVALID                                           dispx                        = dispx                                             dispy                        = dispy                                             dispz                        = dispz                                             long_name                    = Kernels/stressdiv_dispx                           save_in                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = dispx                                           [../]                                                                                                                                                             [./stressdiv_dispy]                                                                type                         = CardiacKirchhoffStressDivergence                  block                        = INVALID                                           component                    = 1                                                 diag_save_in                 = INVALID                                           dispx                        = dispx                                             dispy                        = dispy                                             dispz                        = dispz                                             long_name                    = Kernels/stressdiv_dispy                           save_in                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = dispy                                           [../]                                                                                                                                                             [./stressdiv_dispz]                                                                type                         = CardiacKirchhoffStressDivergence                  block                        = INVALID                                           component                    = 2                                                 diag_save_in                 = INVALID                                           dispx                        = dispx                                             dispy                        = dispy                                             dispz                        = dispz                                             long_name                    = Kernels/stressdiv_dispz                           save_in                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = dispz                                           [../]                                                                          []                                                                                                                                                                [Materials]                                                                                                                                                         [./cardiac_material]                                                               type                         = CardiacNash2000Material                           Ta                           = 0                                                 Ta_function                  = active_tension                                    a_MN                         = '0.523 0.681 1.037 0.731 0.886 0.731'             b_MN                         = '1.351 5.991 0.398 2 2 2'                         block                        = 0                                                 boundary                     = INVALID                                           dispx                        = dispx                                             dispy                        = dispy                                             dispz                        = dispz                                             k_MN                         = '1.937 0.028 0.31 1 1 1'                          long_name                    = Materials/cardiac_material                        output_properties            = INVALID                                           outputs                      = none                                              p                            = hydrostatic_pressure                              use_displaced_mesh           = 0                                               [../]                                                                                                                                                             [./cardiac_properties]                                                             type                         = CardiacFibresMaterial                             block                        = 0                                                 boundary                     = INVALID                                           fixed_R                      = '(xx,xy,xz)=(       1,        0,        0)    (yx,yy,yz)=(       0,        1,        0)                                        (zx,zy,zz)=(       0,        0,        1)'                                           long_name                    = Materials/cardiac_properties                      output_properties            = INVALID                                           outputs                      = none                                              use_displaced_mesh           = 0                                               [../]                                                                          []                                                                                                                                                                [Mesh]                                                                             displacements                  = INVALID                                         block_id                       = INVALID                                         block_name                     = INVALID                                         boundary_id                    = INVALID                                         boundary_name                  = INVALID                                         construct_side_list_from_node_list = 0                                           ghosted_boundaries             = INVALID                                         ghosted_boundaries_inflation   = INVALID                                         patch_size                     = 40                                              second_order                   = 0                                               type                           = GeneratedMesh                                   uniform_refine                 = 0                                               centroid_partitioner_direction = INVALID                                         dim                            = 3                                               distribution                   = DEFAULT                                         elem_type                      = INVALID                                         long_name                      = Mesh                                            nemesis                        = 0                                               nx                             = 1                                               ny                             = 1                                               nz                             = 1                                               partitioner                    = default                                         patch_update_strategy          = never                                           xmax                           = 1                                               xmin                           = 0                                               ymax                           = 1                                               ymin                           = 0                                               zmax                           = 1                                               zmin                           = 0                                             []                                                                                                                                                                [Outputs]                                                                          checkpoint                     = 0                                               color                          = 1                                               console                        = 0                                               csv                            = 0                                               exodus                         = 1                                               file_base                      = INVALID                                         gmv                            = 0                                               gnuplot                        = 0                                               hide                           = INVALID                                         interval                       = 1                                               nemesis                        = 0                                               output_final                   = 0                                               output_if_base_contains        = INVALID                                         output_initial                 = 0                                               output_intermediate            = 1                                               show                           = INVALID                                         solution_history               = 0                                               sync_times                     =                                                 tecplot                        = 0                                               vtk                            = 0                                               xda                            = 0                                               xdr                            = 0                                                                                                                                [./console]                                                                        type                         = Console                                           all_variable_norms           = 0                                                 append_displaced             = 0                                                 append_restart               = 0                                                 end_time                     = INVALID                                           file_base                    = INVALID                                           fit_mode                     = ENVIRONMENT                                       hide                         = INVALID                                           interval                     = 1                                                 linear_residual_dt_divisor   = 1000                                              linear_residual_end_time     = INVALID                                           linear_residual_start_time   = INVALID                                           linear_residuals             = 0                                                 long_name                    = Outputs/console                                   max_rows                     = 15                                                nonlinear_residual_dt_divisor = 1000                                             nonlinear_residual_end_time  = INVALID                                           nonlinear_residual_start_time = INVALID                                          nonlinear_residuals          = 1                                                 outlier_multiplier           = '0.8 2'                                           outlier_variable_norms       = 1                                                 output_failed                = 1                                                 output_file                  = 0                                                 output_final                 = 0                                                 output_if_base_contains      = INVALID                                           output_initial               = 0                                                 output_input                 = 0                                                 output_intermediate          = 1                                                 output_postprocessors        = 1                                                 output_scalar_variables      = 1                                                 output_screen                = 1                                                 output_system_information    = 1                                                 padding                      = 4                                                 perf_header                  = INVALID                                           perf_log                     = 0                                                 scientific_time              = 0                                                 sequence                     = INVALID                                           setup_log                    = INVALID                                           setup_log_early              = 0                                                 show                         = INVALID                                           show_multiapp_name           = 0                                                 solve_log                    = INVALID                                           start_time                   = INVALID                                           sync_only                    = 0                                                 sync_times                   =                                                   time_precision               = INVALID                                           time_tolerance               = 1e-14                                             use_displaced                = 0                                                 verbose                      = 0                                               [../]                                                                                                                                                             [./exodus]                                                                         type                         = Exodus                                            append_displaced             = 0                                                 append_oversample            = 0                                                 elemental_as_nodal           = 0                                                 end_time                     = INVALID                                           file                         = INVALID                                           file_base                    = INVALID                                           hide                         = INVALID                                           interval                     = 1                                                 linear_residual_dt_divisor   = 1000                                              linear_residual_end_time     = INVALID                                           linear_residual_start_time   = INVALID                                           linear_residuals             = 0                                                 long_name                    = Outputs/exodus                                    nonlinear_residual_dt_divisor = 1000                                             nonlinear_residual_end_time  = INVALID                                           nonlinear_residual_start_time = INVALID                                          nonlinear_residuals          = 0                                                 output_elemental_variables   = 1                                                 output_failed                = 0                                                 output_final                 = 0                                                 output_if_base_contains      = INVALID                                           output_initial               = 0                                                 output_input                 = 1                                                 output_intermediate          = 1                                                 output_material_properties   = 0                                                 output_nodal_variables       = 1                                                 output_postprocessors        = 1                                                 output_scalar_variables      = 1                                                 oversample                   = 0                                                 padding                      = 3                                                 position                     = INVALID                                           refinements                  = 0                                                 scalar_as_nodal              = 0                                                 sequence                     = INVALID                                           show                         = INVALID                                           show_material_properties     = INVALID                                           start_time                   = INVALID                                           sync_only                    = 0                                                 sync_times                   =                                                   time_tolerance               = 1e-14                                             use_displaced                = 0                                               [../]                                                                          []                                                                                                                                                                [Postprocessors]                                                                                                                                                    [./volume_ratio]                                                                   type                         = CardiacMaterialVolumeRatioPostprocessor           block                        = INVALID                                           execute_on                   = residual                                          long_name                    = Postprocessors/volume_ratio                       outputs                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                               [../]                                                                          []                                                                                                                                                                [ScalarKernels]                                                                                                                                                     [./incompressibility]                                                              type                         = CardiacIncompressibilityLagrangeMultiplier        long_name                    = ScalarKernels/incompressibility                   use_displaced_mesh           = 0                                                 variable                     = hydrostatic_pressure                              volume_ratio_postprocessor   = volume_ratio                                    [../]                                                                          []                                                                                                                                                                [Variables]                                                                                                                                                         [./dispx]                                                                          block                        = INVALID                                           eigen                        = 0                                                 family                       = LAGRANGE                                          initial_condition            = 0                                                 order                        = FIRST                                             scaling                      = 1                                                 initial_from_file_timestep   = 2                                                 initial_from_file_var        = INVALID                                         [../]                                                                                                                                                             [./dispy]                                                                          block                        = INVALID                                           eigen                        = 0                                                 family                       = LAGRANGE                                          initial_condition            = 0                                                 order                        = FIRST                                             scaling                      = 1                                                 initial_from_file_timestep   = 2                                                 initial_from_file_var        = INVALID                                         [../]                                                                                                                                                             [./dispz]                                                                          block                        = INVALID                                           eigen                        = 0                                                 family                       = LAGRANGE                                          initial_condition            = 0                                                 order                        = FIRST                                             scaling                      = 1                                                 initial_from_file_timestep   = 2                                                 initial_from_file_var        = INVALID                                         [../]                                                                                                                                                             [./hydrostatic_pressure]                                                           block                        = INVALID                                           eigen                        = 0                                                 family                       = SCALAR                                            initial_condition            = 0                                                 order                        = FIRST                                             scaling                      = 1                                                 initial_from_file_timestep   = 2                                                 initial_from_file_var        = INVALID                                         [../]                                                                          []                                                                                ?�              ����t�%�����&?b                ������0e�                ��r�6ؔ�?�ь)� �                ����A�?�3��r��                ������ ˿����S �                ?���?*Ӧ?���w�        ?�     ?Ć�,��