#!/usr/bin/env python

import sys

try:
    import PetscBinaryRead
except ImportError:
    print "This script needs the PetscBinaryRead module. Please download from http://www.stce.rwth-aachen.de/trac/petsc/browser/bin/python/PetscBinaryRead.py"
    sys.exit(42)

import matplotlib.pyplot as plt

if len(sys.argv) == 3:
  fig=plt.figure(figsize=(6,6))
  fig.subplots_adjust(left=0.06,bottom=0.06,top=0.975,right=0.975)
  objects = PetscBinaryRead.readBinaryFile(sys.argv[1],'dense')
  plt.imshow(objects[int(sys.argv[2])]==0,interpolation='nearest',cmap='bone')
  plt.savefig('%s.pdf' % sys.argv[1])
  plt.show()
else:
  print "Usage: %s MATRIXFILE MATRIX#" % sys.argv[0]