[Mesh]
  type = GeneratedMesh
   uniform_refine = 2
   dim           = 1
   nx            = 1
   xmax          =  1.0
   xmin          =  0.0
 []

[Variables]
  [./x] [../]
[]

[Kernels]
  [./diff_x] type = Diffusion variable = x [../]
[]

[AuxVariables]
  [./potential] [../]
[]

[AuxKernels]
  [./potential_func]
    type = FunctionAux
    variable = potential
    # This function mimics the temporal behaviour of the cardiac membrane potential
    # see http://www.wolframalpha.com/input/?i=plot+%281%2F%281%2Bexp%280.5*%28x%2F12-12%29%29%29%2F%281%2Bexp%28-10*%28x%2F12-2%29%29%29-1%29*90.272+for+x+from+0+to+300
    # for a plot or enter "plot (1/(1+exp(0.5*(x/12-12)))/(1+exp(-10*(x/12-2)))-1)*90.272 for x from 0 to 300" on Wolfram Alpha
    function = '(1/(1+exp(0.5*(t/12-12)))/(1+exp(-10*(t/12-2)))-1)*90.272'
  [../]
[]

[Materials]
  [./active_tension]
  type = ActiveTensionODE
  block = 0
  Vmem = 'potential'
  outputs = all
  [../]
[]

[Executioner]
  type = Transient
  start_time = 0
  end_time   = 300
  dtmax      = 1
  dtmin      = 1
[]

[Outputs]
  exodus = true
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
[]
