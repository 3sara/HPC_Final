#!/bin/bash
#SBATCH --no-requeue
#SBATCH --job-name="hpc"
#SBATCH --get-user-env
#SBATCH --partition=EPYC
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=128
#SBATCH --exclusive
#SBATCH --time=02:00:00

module load openMPI/4.1.5/gnu

#export OMP_PLACES=cores
#export OMP_PROC_BIND=close
#export OMP_NUM_THREADS=128

mpicc -o main_6try main_6try.c
mpiexec -np 256 ./main_6try
