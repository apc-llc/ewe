# use with ewe-opt

[Mesh]
      dim           = 3
      distribution  = DEFAULT
      nx            = 16
      ny            = 4
      nz            = 4
      type          = GeneratedMesh
      xmax          = 10.0
      xmin          =  0.0
      ymax          =  0.5
      ymin          = -0.5
      zmax          =  0.5
      zmin          = -0.5
      displacements = 'dispx dispy dispz'
[]

[Variables]
  [./dispx]  order=FIRST  family=LAGRANGE  [../]
  [./dispy]  order=FIRST  family=LAGRANGE  [../]
  [./dispz]  order=FIRST  family=LAGRANGE  [../]
[]

[Kernels]
  [./stressdiv_dispx]  type=CardiacKirchhoffStressDivergence  variable=dispx  component=0  displacements='dispx dispy dispz'  [../]
  [./stressdiv_dispy]  type=CardiacKirchhoffStressDivergence  variable=dispy  component=1  displacements='dispx dispy dispz'  [../]
  [./stressdiv_dispz]  type=CardiacKirchhoffStressDivergence  variable=dispz  component=2  displacements='dispx dispy dispz'  [../]
[]

[Materials]
  active = 'fibres cardiac_material_linear'

  [./fibres]
    type=CardiacFibresMaterial
    block=0
    fixed_R='1 0 0
             0 1 0
             0 0 1'
    outputs=all
    output_properties='E_fibre_x E_fibre_y E_fibre_z E_sheet_x E_sheet_y E_sheet_z E_normal_x E_normal_y E_normal_z'
  [../]

  [./cardiac_material_costa]
    type = CardiacCostaMaterial
    block = 0
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
    block=0
    use_displaced_mesh=false
    # material parameters as given in Table 1 of [Holzapfel 2009]
    #in following order:     a,    b,   a_f,   b_f,  a_s,   b_s, a_fs,  b_fs
    material_parameters='0.059 8.023 18.472 16.026 2.481 11.120 0.216 11.436'
    displacements ='dispx dispy dispz'
    outputs=all
    output_properties='Kirchhoff_stress'
  [../]
  
  [./cardiac_material_holzapfel]
    type = CardiacNash2000Material
    block = 0
    use_displaced_mesh = false
    # material parameters in the order 11 22 33 12 23 31 (symmetric)
    # taken from [Nash & Hunter, 2000], Table I
    k_MN = '1.937 0.028 0.310 1.000 1.000 1.000'
    a_MN = '0.523 0.681 1.037 0.731 0.886 0.731'
    b_MN = '1.351 5.991 0.398 2.000 2.000 2.000'
    displacements ='dispx dispy dispz'
    outputs=all
    output_properties='Kirchhoff_stress'
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
  [./displacement_x]  type=DirichletBC  variable=dispx  boundary=left  value=0.0  [../]
  [./displacement_y]  type=DirichletBC  variable=dispy  boundary=left  value=0.0  [../]
  [./displacement_z]  type=DirichletBC  variable=dispz  boundary=left  value=0.0  [../]

  [./pressure_x]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary='right top bottom front back'  value=0.0  [../]
  [./pressure_y]  type=CardiacMechanicsPressureBC  variable=dispy  component=1  boundary='      top bottom front back'  value=0.0  [../]
  [./pressure_z]  type=CardiacMechanicsPressureBC  variable=dispz  component=2  boundary='right top bottom front back'  value=0.0  [../]

  [./moving_z]
    type = FunctionDirichletBC
    variable = dispy
    boundary = 'right'
    function = pull
  [../]
[]

[Functions]
  [./pull]
    type = ParsedFunction
    value = '5*sin(2*pi*t/5)'
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK
  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                 hypre    boomeramg      4'
  line_search = 'none'


  nl_rel_step_tol = 1.e-8
  l_max_its = 100

  start_time = 0
  end_time   = 5.0
  #num_steps = 10
  dtmax      = 0.1
  dtmin      = 0.1
[]

[Outputs]
  output_on = 'initial timestep_end'
  exodus = true
  
  [./console]
    type = Console
    perf_log = true
    output_on = 'initial nonlinear timestep_begin timestep_end'
  [../]
[]
