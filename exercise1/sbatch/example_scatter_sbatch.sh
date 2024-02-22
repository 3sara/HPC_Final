#!/bin/bash
#SBATCH --job-name=HPC_ex
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=48
#SBATCH --time=02:00:00
#SBATCH --partition=THIN
#SBATCH --exclusive
module load openMPI/4.1.5/gnu/12.2.1
echo "Processes,Size,Latency" > ../../../../../HPCexam/epyc_s0_map_by_core.csv
# Numero di ripetizioni per ottenere una media
repetitions=10000
for j in {2..48}
do
        for i in {1..20}
                do
                size=$((2**i))
                # Esegui osu_bcast con dimensione fissa e numero di processi su due nodi
                result_bcast=$(mpirun --map-by core -np $processes --mca coll_tuned_use_dynamic_rules true --mca coll_tuned_scatter_algorithm 0 osu_scatter -m $size -x $repetitions -i $repetitions | tail -n 1 | awk '{print $2}')
    # Scrivi i risultati nel file CSV
                echo "$processes,$size,$result_bcast"
                echo "$processes,$size,$result_bcast" >> ../../../../../HPCexam/epyc_s0_map_by_core.csv
done
done
