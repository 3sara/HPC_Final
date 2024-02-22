#!/bin/bash
#SBATCH --no-requeue
#SBATCH --job-name="hpc"
#SBATCH --get-user-env
#SBATCH --partition=EPYC
#SBATCH --nodes=2
#SBATCH --cpus-per-task=128
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load openMPI/4.1.5/gnu

#export OMP_PLACES=cores
#export OMP_PROC_BIND=close
#export OMP_NUM_THREADS=128

mpicc -o bcast_tree bcast_tree.c 
mpiexec -n  5 ./bcast_tree

#mpicc -o bcast-hibrid-phind bcast-hibrid-phind.c
#mpiexec -n  4 ./bcast-hibrid-phind



#mpicc -o tornatore_bcast tornatore_bcast.c
#mpiexec -n 4 ./tornatore_bcast
