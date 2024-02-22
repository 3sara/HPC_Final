#include "bcast_tree_no_print.c"
#include "bcast_chain_no_print.c"
#include "bcast_flat.c"
#include <sys/time.h>


int main(int argc, char *argv[]) {
    int my_rank, num_procs;
    int root_rank = 0;
    int data_bintree = 123;
    int data_chain=456;
    int data_flat=789;
    double start_time_bintree, end_time_bintree;
    double start_time_chain, end_time_chain;
    double start_time_flat, end_time_flat;
    double tick;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    tick = MPI_Wtick(); // Ottieni la risoluzione del timer MPI

    start_time_bintree = MPI_Wtime(); // Avvia il timer MPI

    // Esegui il broadcast seguendo l'albero binario
    	binary_tree_broadcast(&data_bintree, my_rank, num_procs, root_rank);
   
    end_time_bintree = MPI_Wtime(); // Ferma il timer MPI

    // Ogni processo stampa i dati ricevuti
   
    start_time_chain = MPI_Wtime(); // Avvia il timer MPI
        chain_broadcast(&data_chain, my_rank, num_procs, root_rank);
    end_time_chain = MPI_Wtime(); // Ferma il timer MPI
   
    start_time_flat =MPI_Wtime();
    	flat_tree_broadcast(&data_flat,my_rank,num_procs,root_rank);
    end_time_flat=MPI_Wtime();
    // Calcola il tempo trascorso per il broadcast
    if (my_rank == root_rank) {
        double elapsed_time_bintree = end_time_bintree - start_time_bintree;
	double elapsed_time_chain=end_time_chain - start_time_chain;
	double elapsed_time_flat=end_time_flat - start_time_flat;
        printf("Time taken by process bintree: %.3f milliseconds\n", (end_time_bintree-start_time_bintree) * 1000);
	printf("MPI timer resolution: %.10f seconds\n", tick);
	printf("Time taken by process chain: %.3f milliseconds\n", (end_time_chain - start_time_chain) * 1000);
	printf("MPI timer resolution: %.10f seconds\n", tick);
	printf("Time taken by process flat: %.3f milliseconds\n", (end_time_flat - start_time_flat) * 1000);
        printf("MPI timer resolution: %.10f seconds\n", tick);
    }

    MPI_Finalize();
    return 0;
}
