#!/bin/bash
#SBATCH --no-requeue
#SBATCH --job-name="hpc"
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=24
#SBATCH --exclusive
#SBATCH --time=02:00:00
#SBATCH --verbose
#SBATCH -A dssc
module load openMPI/4.1.5/gnu

mpicc -o main_strong_scaling main_strong_scaling.c
mpiexec -np 48 ./main_strong_scaling
