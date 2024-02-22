#include "bcast_tree_no_print.c"
#include "bcast_chain_no_print.c"
#include <sys/time.h>
#include <stdlib.h>

#define DATA_SIZE 1000000 // Dimensione dei dati da trasmettere

int main(int argc, char *argv[]) {
    int my_rank, num_procs;
    int root_rank = 0;
    double start_time_bintree, end_time_bintree;
    double start_time_chain, end_time_chain;
    double tick;
    int *data_bintree = NULL;
    int *data_chain = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    tick = MPI_Wtick(); // Ottieni la risoluzione del timer MPI

    // Allocazione e inizializzazione dei dati per l'albero binario
    data_bintree = (int*)malloc(DATA_SIZE * sizeof(int));
    if (data_bintree == NULL) {
        fprintf(stderr, "Errore durante l'allocazione della memoria per i dati bintree\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    for (int i = 0; i < DATA_SIZE; i++) {
        data_bintree[i] = i;
    }

    start_time_bintree = MPI_Wtime(); // Avvia il timer MPI
    binary_tree_broadcast(data_bintree, my_rank, num_procs, root_rank);
    end_time_bintree = MPI_Wtime(); // Ferma il timer MPI

    // Allocazione e inizializzazione dei dati per la catena
    data_chain = (int*)malloc(DATA_SIZE * sizeof(int));
    if (data_chain == NULL) {
        fprintf(stderr, "Errore durante l'allocazione della memoria per i dati chain\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    for (int i = 0; i < DATA_SIZE; i++) {
        data_chain[i] = i;
    }

    start_time_chain = MPI_Wtime(); // Avvia il timer MPI
    chain_broadcast(data_chain, my_rank, num_procs, root_rank);
    end_time_chain = MPI_Wtime(); // Ferma il timer MPI

    // Calcola il tempo trascorso per il broadcast
    if (my_rank == root_rank) {
        double elapsed_time_bintree = end_time_bintree - start_time_bintree;
        double elapsed_time_chain = end_time_chain - start_time_chain;
        printf("Elapsed time for broadcast binary tree: %.10f seconds\n", elapsed_time_bintree);
        printf("Elapsed time for broadcast chain: %.10f seconds\n", elapsed_time_chain);
    }

    // Libera la memoria allocata
    free(data_bintree);
    free(data_chain);

    MPI_Finalize();
    return 0;
}

