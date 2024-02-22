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

mpicc -o main_weak_scaling main_weak_scaling.c

# Apri un file CSV in modalità scrittura (se non esiste verrà creato)
echo "Indice,Risultato" > weak_scaling_thin_MPI.csv

# Ciclo for per eseguire il programma con diversi valori di input
for ((i=2; i<=48; i+=2)); do
    # Esegui il programma e cattura l'output in una variabile
    RISULTATO=$(mpiexec -np $i ./main_weak_scaling)
    # Scrivi l'indice e il risultato nel file CSV
    echo "$i,$RISULTATO" >> weak_scaling_thin_MPI.csv
done

