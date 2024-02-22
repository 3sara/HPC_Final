#include "bcast_tree_no_print.c"
#include "bcast_chain_no_print.c"
#include "bcast_flat.c"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define WARMUP_ITERATIONS  1000
#define NUM_RUNS 1000

int main(int argc, char *argv[]) {
    int my_rank, num_procs;
    int root_rank = 0;
    double start_time_bintree, end_time_bintree;
    double start_time_chain, end_time_chain;
    double start_time_flat, end_time_flat;
    double tick;
    int *data= NULL;
    FILE *fp;
    char filename[] = "test.csv";
    long long int size;
    MPI_Init(&argc, &argv);
    int warmup_value =  0;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    tick = MPI_Wtick(); // Ottieni la risoluzione del timer MPI

    int dummy_data = 0;
        // Warmup communication
    for (int i = 0; i < WARMUP_ITERATIONS; ++i) {
        binary_tree_broadcast(&dummy_data, my_rank, num_procs, root_rank,1);
        chain_broadcast(&dummy_data, my_rank, num_procs, root_rank,1);
        flat_tree_broadcast(&dummy_data, my_rank, num_procs, root_rank,1);
    }

    // Apri il file CSV per scrivere i risultati
    fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Errore durante l'apertura del file %s\n", filename);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Intestazione del file CSV
    fprintf(fp, "warmup, barrrier, reduce, flat\n");
    fprintf(fp, "Size,Time_bintree,Time_chain, Time_flat\n");


    // Misura i tempi di esecuzione per diverse dimensioni dei dati
    for (int pow =  1; pow <=  20; ++pow) {
        size =  1ULL << pow;
        double total_time_bintree = 0.0;
        double total_time_chain = 0.0;
        double total_time_flat=0.0;
        // Allocazione e inizializzazione dei dati per l'albero binario
        data = (int*)malloc(size * sizeof(int));
        if (data == NULL) {
            fprintf(stderr, "Errore durante l'allocazione della memoria per i dati\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
	if (my_rank == 0) {
        for (int j = 0; j< size; j++) {
            data[j] = j;
        }}
        // Esegui NUM_RUNS volte per ogni dimensione dei dati
        for (int i = 0; i < NUM_RUNS; i++) {

            MPI_Barrier(MPI_COMM_WORLD);
            // Esegui il broadcast seguendo l'albero binario
            start_time_bintree = MPI_Wtime(); // Avvia il timer MPI
            binary_tree_broadcast(data, my_rank, num_procs, root_rank,size);
            end_time_bintree = MPI_Wtime(); // Ferma il timer MPI
            total_time_bintree += end_time_bintree - start_time_bintree;

            MPI_Barrier(MPI_COMM_WORLD);
            // Esegui il broadcast seguendo la catena
            start_time_chain = MPI_Wtime(); // Avvia il timer MPI
            chain_broadcast(data, my_rank, num_procs, root_rank,size);
            end_time_chain = MPI_Wtime(); // Ferma il timer MPI
            total_time_chain += end_time_chain - start_time_chain;

            MPI_Barrier(MPI_COMM_WORLD);
            start_time_flat = MPI_Wtime(); // Avvia il timer MPI
            flat_tree_broadcast(data, my_rank, num_procs, root_rank,size);
            end_time_flat=MPI_Wtime();
            total_time_flat += end_time_flat - start_time_flat;

        }
        // Libera la memoria allocata
        free(data);
        // Calcola il tempo medio per l'albero binario e la catena
        double avg_time_bintree = total_time_bintree / NUM_RUNS;
        double avg_time_chain = total_time_chain / NUM_RUNS;
        double avg_time_flat=total_time_flat /NUM_RUNS;

        double total_avg_time_bintree;
        double total_avg_time_chain;
        double total_avg_time_flat;
        MPI_Reduce(&avg_time_bintree, &total_avg_time_bintree, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&avg_time_chain, &total_avg_time_chain, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&avg_time_flat,&total_avg_time_flat, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        // Scrivi i risultati nel file CSV
        if (my_rank == 0) {
            fprintf(fp, "%lld,%f,%f,%f\n", size, total_avg_time_bintree * 1000, total_avg_time_chain * 1000,total_avg_time_flat*1000);
        }
    }
    // Chiudi il file CSV
    fclose(fp);

    MPI_Finalize();
    return 0;
}
