[Mesh]
      type          = GeneratedMesh
      dim           = 3
      distribution  = DEFAULT
      nx = 1  ny = 1  nz = 1
      xmax = 1.0   xmin = 0.0
      ymax = 0.1   ymin = 0.0
      zmax = 0.1   zmin = 0.0
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
  [./stressdiv_dispx]  type=CardiacKirchhoffStressDivergence  variable=dispx  component=0  displacements='dispx dispy dispz'  p=pressure  [../]
  [./stressdiv_dispy]  type=CardiacKirchhoffStressDivergence  variable=dispy  component=1  displacements='dispx dispy dispz'  p=pressure  [../]
  [./stressdiv_dispz]  type=CardiacKirchhoffStressDivergence  variable=dispz  component=2  displacements='dispx dispy dispz'  p=pressure  [../]
  [./incompressibility]  type=CardiacKirchhoffIncompressibilityLagrangeMultiplier  variable=pressure  displacements='dispx dispy dispz'  [../]
[]

[Materials]
  [./fibres]
    type=CardiacFibresMaterial
    block=all
    fixed_R='1 0 0   0 1 0   0 0 1'
  [../]

  [./cardiac_material_costa]
    type = CardiacCostaMaterial
    block = all
    use_displaced_mesh = false
    # material parameters in the order C, b_f, b_t, b_fs (C in kPa, b unitless)
    material_parameters =           '  2  8    2    4'
    maximumQ = 15
    displacements ='dispx dispy dispz'
    hydrostatic_pressure=pressure
  [../]
[]

[BCs]
  [./d_x]  type=PresetBC  variable=dispx  boundary=left  value=0.0  [../]
  [./d_y]  type=PresetBC  variable=dispy  boundary=left  value=0.0  [../]
  [./d_z]  type=PresetBC  variable=dispz  boundary=left  value=0.0  [../]

  [./pressure_x]  type=CardiacMechanicsPressureBC  variable=dispx  component=0  boundary='bottom'  value=0.004  [../]
  [./pressure_y]  type=CardiacMechanicsPressureBC  variable=dispy  component=1  boundary='bottom'  value=0.004  [../]
  [./pressure_z]  type=CardiacMechanicsPressureBC  variable=dispz  component=2  boundary='bottom'  value=0.004  [../]
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-mat_view -viewer_binary_filename'
  petsc_options_value = ' binary    jacobian_dispx-dispALL'
[]

[Preconditioning]
  [./my_smp]
    # since we are using solve_type=NEWTON instead of PJFNK, this block should be ignored but it isn't:
    # it still determines how the Jacobian looks like when evaluated by PETSc. The default is to only
    # include the diagonal block elements which will for sure be wrong if the Jacobian is directly
    # used for finding the Newton search direction instead of as a preconditioner as it is originally intended
    type = SMP
    #                                                        lower left Jacobian block    upper right Jacobian block
    off_diag_row =    'dispx dispx' # dispy dispy dispz dispz   pressure pressure pressure   dispx    dispy    dispz'
    off_diag_column = 'dispy dispz' # dispx dispz dispx dispy   dispx    dispy    dispz      pressure pressure pressure'
    # full=true below is identical to omitting off_diag_row/column and just including all off-diagonal blocks
    #full = true
  [../]
[]