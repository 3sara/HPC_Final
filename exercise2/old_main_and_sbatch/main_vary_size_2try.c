#include "bcast_tree_no_print.c"
#include "bcast_chain_no_print.c"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_RUNS 100
int main(int argc, char *argv[]) {
    int my_rank, num_procs;
    int root_rank = 0;
    double start_time_bintree, end_time_bintree;
    double start_time_chain, end_time_chain;
    double tick;
    int *data= NULL;
    FILE *fp;
    char filename[] = "results_pow2.csv";
    long long int size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    tick = MPI_Wtick(); // Ottieni la risoluzione del timer MPI

    // Apri il file CSV per scrivere i risultati
    fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Errore durante l'apertura del file %s\n", filename);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    //	INSERIRE WARMUP COMUNICAZIONE
    //	// Intestazione del file CSV
    fprintf(fp, "Size,Time_bintree,Time_chain\n");

    // Misura i tempi di esecuzione per diverse dimensioni dei dati
    for (int i =  1; i <=  20; ++i) {
        size =  1ULL << i; 
        double total_time_bintree = 0.0;
        double total_time_chain = 0.0;

        // Esegui NUM_RUNS volte per ogni dimensione dei dati
        for (int i = 0; i < NUM_RUNS; i++) {
            // Allocazione e inizializzazione dei dati per l'albero binario
            data = (int*)malloc(size * sizeof(int));
            if (data == NULL) {
                fprintf(stderr, "Errore durante l'allocazione della memoria per i dati bintree\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
                for (int i = 0; i < size; i++) {
     		   data[i] = i;
    		}
		//INSERIRE BARRIER
	    MPI_Barrier()
            // Esegui il broadcast seguendo l'albero binario
            start_time_bintree = MPI_Wtime(); // Avvia il timer MPI
            binary_tree_broadcast(data, my_rank, num_procs, root_rank);
            end_time_bintree = MPI_Wtime(); // Ferma il timer MPI
            total_time_bintree += end_time_bintree - start_time_bintree;
	    
	    MPI_Barrier()
            // Esegui il broadcast seguendo la catena
            start_time_chain = MPI_Wtime(); // Avvia il timer MPI
            chain_broadcast(data, my_rank, num_procs, root_rank);
            end_time_chain = MPI_Wtime(); // Ferma il timer MPI
            total_time_chain += end_time_chain - start_time_chain;

            // Libera la memoria allocata
            free(data);
        }
	//INSERIRE REDUCE\:
        // Calcola il tempo medio per l'albero binario e la catena
        double avg_time_bintree = total_time_bintree / NUM_RUNS;
        double avg_time_chain = total_time_chain / NUM_RUNS;

        // Scrivi i risultati nel file CSV
        fprintf(fp, "%d,%f,%f\n", size, avg_time_bintree*1000, avg_time_chain*1000);
    }

    // Chiudi il file CSV
    fclose(fp);

    MPI_Finalize();
    return 0;
}

