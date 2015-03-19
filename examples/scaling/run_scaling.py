import os, math, numpy

Nprocs = [1, 4, 8, 12, 16, 20, 24]

for i in range(0,numpy.size(Nprocs)):
  Nproc = Nprocs[i]
  with open("submit_ewe_Np"+str(Nproc)+".sh", "w") as myfile:
    jobname = "ewe_Np"+str(Nproc)
    Nnodes = int(math.ceil(float(Nproc)/float(8.0)))
    myfile.write("#!/bin/bash \n")
    myfile.write("#SBATCH --job-name="+jobname+"\n")
    myfile.write("#SBATCH --nodes="+str(Nnodes)+"\n")
    myfile.write("#SBATCH --ntasks="+str(Nproc)+"\n")
    myfile.write("#SBATCH --ntasks-per-node=8 \n")
    myfile.write("#SBATCH --cpus-per-task=1 \n")
    myfile.write("#SBATCH --time=08:00:00 \n")
    myfile.write("#SBATCH --output="+jobname+".out \n")
    myfile.write("echo JobID $SLURM_JOB_ID \n")
    myfile.write("echo Nodes $SLURM_NNODES \n")
    myfile.write("echo Tasks $SLURM_NTASKS \n")
    myfile.write("/usr/bin/time mpirun -n "+str(Nproc)+" ../../ewe-opt --no-color -i coupled_bar.i \n")
    myfile.close()
  os.system("sbatch submit_ewe_Np"+str(Nproc)+".sh")