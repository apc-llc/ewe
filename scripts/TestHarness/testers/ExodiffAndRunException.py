# This is a conglomerate of the Exodiff and RunException Testers:
# It verifies that a run has failed with a specific message and has
# produced an exodus file that is identical to the given gold reference
from RunApp import RunApp
from util import runCommand
import os

class ExodiffAndRunException(RunApp):

  @staticmethod
  def validParams_Exodiff(params):
    # Exodiff params
    params.addRequiredParam('exodiff',   [], "A list of files to exodiff.")
    params.addParam('exodiff_opts',      [], "Additional arguments to be passed to invocations of exodiff.")
    params.addParam('gold_dir',      'gold', "The directory where the \"golden standard\" files reside relative to the TEST_DIR: (default: ./gold/)")
    params.addParam('abs_zero',       1e-10, "Absolute zero cutoff used in exodiff comparisons.")
    params.addParam('rel_err',       5.5e-6, "Relative error value used in exodiff comparisons.")
    params.addParam('custom_cmp',            "Custom comparison file")
    params.addParam('use_old_floor',  False, "Use Exodiff old floor option")
    params.addParam('delete_output_before_running',  True, "Delete pre-existing output files before running test. Only set to False if you know what you're doing!")
    return params

  @staticmethod
  def validParams_RunException(params):
    # RunException params
    params.addParam('expect_err', "A regular expression that must occur in the ouput. (Test may terminiate unexpectedly and be considered passing)")
    params.addParam('expect_assert', "DEBUG MODE ONLY: A regular expression that must occur in the ouput. (Test may terminiate unexpectedly and be considered passing)")
    params.addParam('should_crash', True, "Inidicates that the test is expected to crash or otherwise terminate early")
    # Printing errors in parallel often intertwine when multiple processors receive the same error.  We will set max_parallel = 1 by default, but it can be overridden
    params['max_parallel'] = 1
    return params

  @staticmethod
  def validParams():
    params = RunApp.validParams()
    params = ExodiffAndRunException.validParams_Exodiff(params)
    params = ExodiffAndRunException.validParams_RunException(params)

    return params

  def __init__(self, name, params):
    RunApp.__init__(self, name, params)

  def prepare_Exodiff(self):
    if self.specs['delete_output_before_running'] == True:
      for file in self.specs['exodiff']:
        try:
          os.remove(os.path.join(self.specs['test_dir'], file))
        except:
          pass

  def prepare_RunException(self):
    return

  def prepare(self):
    self.prepare_Exodiff()
    self.prepare_RunException()

  def checkRunnable_Exodiff(self, options):
    return (True, '')

  def checkRunnable_RunException(self, options):
    if options.enable_recover:
      reason = 'skipped (RunException RECOVER)'
      return (False, reason)
    return (True, '')

  def checkRunnable(self, options):
    (result, reason) = self.checkRunnable_Exodiff(options)
    if result != True:
      return (result, reason)
    return self.checkRunnable_RunException(options)

  def processResultsCommand_Exodiff(self, moose_dir, options):
    commands = []

    for file in self.specs['exodiff']:
      custom_cmp = ''
      old_floor = ''
      if self.specs.isValid('custom_cmp'):
         custom_cmp = ' -f ' + os.path.join(self.specs['test_dir'], self.specs['custom_cmp'])
      if self.specs['use_old_floor']:
         old_floor = ' -use_old_floor'

      commands.append(os.path.join(moose_dir, 'framework', 'contrib', 'exodiff', 'exodiff') + ' -m' + custom_cmp + ' -F' + ' ' + str(self.specs['abs_zero']) \
                      + old_floor + ' -t ' + str(self.specs['rel_err']) + ' ' + ' '.join(self.specs['exodiff_opts']) + ' ' \
                      + os.path.join(self.specs['test_dir'], self.specs['gold_dir'], file) + ' ' + os.path.join(self.specs['test_dir'], file))

      return commands

  def processResultsCommand_RunException(self, moose_dir, options):
    return []

  def processResultsCommand(self, moose_dir, options):
    res = []
    res = res + self.processResultsCommand_Exodiff(moose_dir, options)
    res = res + self.processResultsCommand_RunException(moose_dir, options)
    return res

  def processResults_RunException(self, moose_dir, retcode, options, output):
    reason = ''
    specs = self.specs

    # Expected errors and assertions might do a lot of things including crash so we
    # will handle them seperately
    if specs.isValid('expect_err'):
      if not self.checkOutputForPattern(output, specs['expect_err']):
        reason = 'NO EXPECTED ERR'
    elif specs.isValid('expect_assert'):
      if options.method == 'dbg':  # Only check asserts in debug mode
        if not self.checkOutputForPattern(output, specs['expect_assert']):
          reason = 'NO EXPECTED ASSERT'

    if reason == '':
      (reason, output) = RunApp.processResults(self, moose_dir, retcode, options, output)

    return (reason, output)

  def processResults(self, moose_dir, retcode, options, output):
    (reason, output) = self.processResults_RunException(moose_dir, retcode, options, output)

    if reason != '' or self.specs['skip_checks']:
      return (reason, output)

    # Don't Run Exodiff on Scaled Tests
    if options.scaling and self.specs['scale_refine']:
      return (reason, output)

    # Make sure that all of the Exodiff files are actually available
    for file in self.specs['exodiff']:
      if not os.path.exists(os.path.join(self.specs['test_dir'], self.specs['gold_dir'], file)):
        output += "File Not Found: " + os.path.join(self.specs['test_dir'], self.specs['gold_dir'], file)
        reason = 'MISSING GOLD FILE'
        break

    # Retrieve the commands
    commands = self.processResultsCommand(moose_dir, options)

    for command in commands:
      exo_output = runCommand(command)

      output += 'Running exodiff: ' + command + '\n' + exo_output + ' ' + ' '.join(self.specs['exodiff_opts'])

      if ('different' in exo_output or 'ERROR' in exo_output) and not "Files are the same" in exo_output:
        reason = 'EXODIFF'
        break

    return (reason, output)
