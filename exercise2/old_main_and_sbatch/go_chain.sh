#!/bin/bash
#SBATCH --no-requeue
#SBATCH --job-name="hpc"
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=24
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load openMPI/4.1.5/gnu

#export OMP_PLACES=cores
#export OMP_PROC_BIND=close
#export OMP_NUM_THREADS=128

mpicc -o bcast_chain bcast_chain.c
mpiexec -np 48 ./bcast_chain
