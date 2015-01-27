#!/usr/bin/env python

import sys
import re as re
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm, Normalize
import matplotlib.cm as cm

if len(sys.argv) != 2:
  print "Usage: %s FILENAME" % sys.argv[0]
  sys.exit(42)
  
filename = sys.argv[1]
n_dof = -1

f = open(filename)
lines = f.readlines()

currline = 0

p_block = re.compile(r'\(\s*(\d*)\s*,\s*([\d\.+-]*)\s*\)')
mat_index=0
matlist = ['bla']
addmat = 0

fig=plt.figure(figsize=(16,12))
fig.subplots_adjust(left=0.06,bottom=0.06,top=0.975,right=0.975)

cmap = cm.jet
cmap.set_bad(alpha = 0.0)

while currline < len(lines):
  line = lines[currline]
  
  if re.search("Nonlinear System:", line):
    currline += 1
    line = lines[currline]
    m = re.search(r'Num DOFs:\s*(\d*)', line)
    n_dof = int(m.group(1))
    print "Identified number of DOF: %d" % n_dof

  if re.search("Mat Object:", line):
    #found a matrix
    mat_name = lines[currline-1].strip('\n')
    mat = np.zeros((n_dof, n_dof))
    mat_index += 1
    print "Found matrix [%s]" % mat_name
    currline += 1 # now we point at " type: .." line
    currline += 1 # now we point at first line of matrix
    for M in range(n_dof):
      mat_line = lines[currline + M]
      for match in p_block.finditer(mat_line):
        col = int(match.group(1))
        val = float(match.group(2))
        mat[M,col] = val
    
    if (mat_index % 3 == 0):
      mat = np.ma.array(mat, mask=abs(mat)<1.e-6)
    
    matlist.append(mat)
    ax = plt.subplot(3,4,mat_index+addmat)
    ax.set_title(mat_name)
    absmat = mat
    plt.imshow(absmat,interpolation='nearest',norm=Normalize(vmin=absmat.min(), vmax=absmat.max()), cmap=cmap)
    plt.colorbar()
    
    #print np.min(mat[np.nonzero(mat)]), np.max(mat[np.nonzero(mat)])
    
    currline += n_dof # point behind the matrix
    
    if mat_index % 3 == 0:
      # add quotient of the matrices
      absmat = np.ma.array(matlist[mat_index-1] / matlist[mat_index-1]-matlist[mat_index-2], mask=(abs(matlist[mat_index-1]-matlist[mat_index-2])<1e-6))
      addmat += 1
      ax = plt.subplot(3,4,mat_index+addmat)
      ax.set_title('Jacobian difference / Finite differences Jacobian')
      
      absmat = np.ma.array(absmat, mask=np.logical_or(np.isnan(absmat), abs(absmat)<1e-6))

      plt.imshow(absmat,interpolation='nearest',norm=Normalize(vmin=np.min(absmat[np.nonzero(absmat)]), vmax=absmat.max()), cmap=cmap)
      plt.colorbar()

  else:
    currline += 1
  
plt.savefig("%s.pdf" % filename)
plt.show()