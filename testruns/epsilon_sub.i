[Mesh]
  file = 07-heart_geometry_new.e
  block_id = '1'
  block_name = 'all'
  displacements = 'dispx dispy dispz'
  # uniform_refine = 1
[]

[Variables]
  [./potential]  order=FIRST  family=LAGRANGE  [../]
[]

[AuxVariables]
  [./gate_m]   order=CONSTANT  family = MONOMIAL  [../]
  [./gate_v]   order=CONSTANT  family = MONOMIAL  [../]
  [./gate_f]   order=CONSTANT  family = MONOMIAL  [../]
  [./gate_to]  order=CONSTANT  family = MONOMIAL  [../]
  [./gate_x]   order=CONSTANT  family = MONOMIAL  [../]
  [./dispx]    order=FIRST     family = LAGRANGE  [../]
  [./dispy]    order=FIRST     family = LAGRANGE  [../]
  [./dispz]    order=FIRST     family = LAGRANGE  [../]
  #
  [./distance_outer]      order=FIRST  family=LAGRANGE [../]
  [./distance_RV_inner]   order=FIRST  family=LAGRANGE [../]
  [./distance_LV_inner]   order=FIRST  family=LAGRANGE [../]
  [./thickness_parameter] order=FIRST  family=LAGRANGE [../]
[]

[AuxKernels]
  [./auxgate_m]   type=MaterialStdVectorAux  property='gates'  variable=gate_m   block=all  index=0  execute_on='timestep_end'  [../]
  [./auxgate_v]   type=MaterialStdVectorAux  property='gates'  variable=gate_v   block=all  index=1  execute_on='timestep_end'  [../]
  [./auxgate_f]   type=MaterialStdVectorAux  property='gates'  variable=gate_f   block=all  index=2  execute_on='timestep_end'  [../]
  [./auxgate_to]  type=MaterialStdVectorAux  property='gates'  variable=gate_to  block=all  index=3  execute_on='timestep_end'  [../]
  [./auxgate_x]   type=MaterialStdVectorAux  property='gates'  variable=gate_x   block=all  index=4  execute_on='timestep_end'  [../]
  #
  [./auxdistance_outer]     type=VolumeNearestNodeDistanceAux  variable=distance_outer     block=all  paired_boundary=ss_outer     execute_on='initial' [../]
  [./auxdistance_RV_inner]  type=VolumeNearestNodeDistanceAux  variable=distance_RV_inner  block=all  paired_boundary=ss_RV_inner  execute_on='initial' [../]
  [./auxdistance_LV_inner]  type=VolumeNearestNodeDistanceAux  variable=distance_LV_inner  block=all  paired_boundary=ss_LV_inner  execute_on='initial' [../]

  [./auxthickness]
    type = CardiacThicknessParameterAux
    variable = thickness_parameter
    distance_RV_inner = distance_RV_inner
    distance_LV_inner = distance_LV_inner
    distance_outer    = distance_outer
    execute_on='initial'
  [../]
[]

[Kernels]
  [./diff]
    type = ElectrocardioDiffusion
    variable = potential
    surface_to_volume = 1000.0
    capacitance = 1.0
    use_displaced_mesh = true
  [../]
  
  [./ecforcing]
    type = ElectrocardioForcing
    variable = potential
    forcing_function = ElectrocardioForcing_function
    #ion_coeff = 0.0
    use_displaced_mesh = true
  [../]

  [./euler]
    type = ElectrocardioTimeDerivative
    variable = potential
    use_displaced_mesh = true
  [../]
[]

[Functions]
  [./ElectrocardioForcing_function]
    type = PiecewiseFunction
    default_function = '0'
    functions = 'PurkinjePacing'
    left      = ' 0.0'
    right     = ' 5.0'
  [../]

  [./PurkinjePacing]
    # we are pacing at the following "early activation sites" which were hand-picked in Paraview
    # to closely match [Potse2006, Figure 1]:
    #   0.8 6.4 -3.1
    #  -1.4 6.4 -2.9
    #   2.2 8.9 -4.2
    type = ParsedFunction
    value = 'A*( exp(-0.5*(pow(x-0.8,2)+pow(y-6.4,2)+pow(z+3.1,2))/pow(w,2))
                +exp(-0.5*(pow(x+1.4,2)+pow(y-6.4,2)+pow(z+2.9,2))/pow(w,2))
                +exp(-0.5*(pow(x-2.2,2)+pow(y-8.9,2)+pow(z+4.2,2))/pow(w,2)))'
    vars = '  A   w' # amplitude and spatial width of the Gaussians
    vals = '-30 1.5'
  [../]
[]

[BCs]
  [./bc]
    type = NeumannBC
    variable = potential
    boundary = 'ss_LV_inner ss_RV_inner ss_outer'
    value = 0
  [../]
[]

[ICs]
  [./ec]
    type = ElectrocardioIC
    variable = potential
    block = all
  [../]
[]

[Materials]
 [./fibres]
    type = CardiacFibresMaterial
    block = all
    thickness_parameter=thickness_parameter
    outputs=all
    output_properties='E_fibre_x E_fibre_y E_fibre_z E_sheet_x E_sheet_y E_sheet_z E_normal_x E_normal_y E_normal_z'
  [../]
  
  [./electrocardio]
    type = Electrocardio
    vmem = 'potential'
    block = all
    outputs = all
    #use_displaced_mesh = true # TODO: if activated I am getting a segfault
  [../]
 
  [./conductivity]
    type = ElectrocardioConductivity
    conductivities = '0.0012 0.003 0.003'
    block = all
    #use_displaced_mesh = true # TODO: if activated I am getting a segfault
  [../]
[]

[Executioner]
  type = Transient   # Here we use the Transient Executioner
  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  #l_tol = 1e-6
  #l_max_its = 10
  nl_rel_tol = 1e-3
  nl_abs_tol = 1e-6
  nl_rel_step_tol = 1e-6
  nl_max_its = 2

  start_time = 0
  end_time   = 500.0
  dtmin = 0.25
  dtmax = 0.25
  scheme = 'implicit-euler'
[]

 
[Outputs]
 [./console]
    type=Console
    perf_log=true
    output_on = 'initial nonlinear linear timestep_begin timestep_end'
    hide = 'distance_outer distance_RV_inner distance_LV_inner'
  [../]
  
  [./electro]
     type=Exodus
     output_on = 'timestep_end'
     hide = 'distance_outer distance_RV_inner distance_LV_inner'
  [../]
[]
