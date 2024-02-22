#!/bin/bash
#SBATCH --job-name="hpc"
#SBATCH --get-user-env
#SBATCH --partition=THIN
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=24
#SBATCH --exclusive
#SBATCH --time=02:00:00
##SBATCH --nodelist=EPYC002

module load openMPI/4.1.5/gnu/12.2.1

#export OMP_PLACES=cores
#export OMP_PROC_BIND=close
#export OMP_NUM_THREADS=128

mpicc -o main_9try_bis main_9try_bis.c
mpiexec -np 48 ./main_9try_bis
