#include "bcast_tree.c"  
#include "bcast_chain.c" 
#include "bcast_flat.c"  
#include <sys/time.h>    
#include <stdlib.h>      
#include <stdio.h>       

#define WARMUP_ITERATIONS  1000 // Number of warmup iterations
#define NUM_RUNS 1000            // Number of total runs

int main(int argc, char *argv[]) {
    int my_rank, num_procs;
    int root_rank = 0;
    double start_time_bintree, end_time_bintree;
    double start_time_chain, end_time_chain;
    double start_time_flat, end_time_flat;
    double start_time_bcast, end_time_bcast;
    int *data= NULL;
    char filename[] = "weak_scaling.csv";
    long long int size;
    MPI_Init(&argc, &argv); // Initialize MPI
    int warmup_value =  0;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // Get rank of current process
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // Get total number of processes
    
    int dummy_data = 0;
        // Warmup communication
    for (int i = 0; i < WARMUP_ITERATIONS; ++i) {
        binary_tree_broadcast(&dummy_data, my_rank, num_procs, root_rank,1);
        chain_broadcast(&dummy_data, my_rank, num_procs, root_rank,1);
	    flat_tree_broadcast(&dummy_data, my_rank, num_procs, root_rank,1);
    }


	// Fixed data size to 2^10 
    size =  1ULL << 20;
    double total_time_bintree = 0.0;
    double total_time_chain = 0.0;
	double total_time_flat=0.0;
	double total_time_bcast=0.0;

	// Allocation and initialization of data for binary tree
    data = (int*)malloc(size * sizeof(int));
    if (data == NULL) {
        fprintf(stderr, "Error allocating memory for data\n");
        MPI_Abort(MPI_COMM_WORLD, 1); // Abort MPI execution
    }
    if (my_rank == 0) {
    for (int j = 0; j< size; j++) {
        data[j] = j;
    }}
// Execute NUM_RUNS times for each data size
    for (int i = 0; i < NUM_RUNS; i++) {

        MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes
        // Execute broadcast using binary tree
        start_time_bintree = MPI_Wtime(); // Start MPI timer
        binary_tree_broadcast(data, my_rank, num_procs, root_rank,size);
        end_time_bintree = MPI_Wtime(); // Stop MPI timer
        total_time_bintree += end_time_bintree - start_time_bintree;

        MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes
        // Execute broadcast using chain
        start_time_chain = MPI_Wtime(); // Start MPI timer
        chain_broadcast(data, my_rank, num_procs, root_rank,size);
        end_time_chain = MPI_Wtime(); // Stop MPI timer
        total_time_chain += end_time_chain - start_time_chain;

        MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes
        start_time_flat = MPI_Wtime(); // Start MPI timer
        flat_tree_broadcast(data, my_rank, num_procs, root_rank,size);
        end_time_flat=MPI_Wtime(); // Stop MPI timer
        total_time_flat += end_time_flat - start_time_flat;
	
	MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes
        start_time_bcast = MPI_Wtime(); // Start MPI timer
        MPI_Bcast(data,size,MPI_INT,root_rank,MPI_COMM_WORLD);
        end_time_bcast=MPI_Wtime(); // Stop MPI timer
        total_time_bcast += end_time_bcast - start_time_bcast;
    }
    // Free allocated memory
    free(data);
    // Calculate average time for binary tree and chain
    double avg_time_bintree = total_time_bintree / NUM_RUNS;
    double avg_time_chain = total_time_chain / NUM_RUNS;
	double avg_time_flat=total_time_flat /NUM_RUNS;
	double avg_time_bcast=total_time_bcast / NUM_RUNS;
    double max_time_bintree;
    double max_time_chain;
	double max_time_flat;
	double max_time_bcast;
    MPI_Reduce(&avg_time_bintree, &max_time_bintree, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); // Reduce and get maximum time for binary tree
    MPI_Reduce(&avg_time_chain, &max_time_chain, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); // Reduce and get maximum time for chain
	MPI_Reduce(&avg_time_flat,&max_time_flat, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); // Reduce and get maximum time for flat tree
        
	MPI_Reduce(&avg_time_bcast,&max_time_bcast, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 
	// Write results to CSV file
    if (my_rank ==  0) {
        printf("%lld,%f,%f,%f, %f\n", size, max_time_bintree *  1000, max_time_chain *  1000, max_time_flat *  1000, max_time_bcast *1000);
    }

    MPI_Finalize(); // Finalize MPI

    return 0;
}
