#include "bcast_tree.c"
#include "bcast_chain.c"
#include "bcast_flat.c"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define WARMUP_ITERATIONS 1000
#define NUM_RUNS 1000

int main(int argc, char *argv[]) {
    int my_rank, num_procs;
    int root_rank = 0;
    double start_time_bintree, end_time_bintree;
    double start_time_chain, end_time_chain;
    double start_time_flat, end_time_flat;
    double start_time_bcast, end_time_bcast;
    int *data = NULL;
    FILE *fp;
    char filename[] = "strong_scaling_epyc_with_MPI.csv";
    long long int size;
    MPI_Init(&argc, &argv);
    int warmup_value = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int dummy_data = 0;
    // Warmup communication
    for (int i = 0; i < WARMUP_ITERATIONS; ++i) {
        binary_tree_broadcast(&dummy_data, my_rank, num_procs, root_rank, 1);
        chain_broadcast(&dummy_data, my_rank, num_procs, root_rank, 1);
        flat_tree_broadcast(&dummy_data, my_rank, num_procs, root_rank, 1);
    }

    // Open CSV file to write results
    fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // CSV file header
    fprintf(fp, "size,bintree,chain,flat\n");

    // Measure execution times for different data sizes
    for (int pow = 0; pow <= 20; ++pow) {
        size = 1ULL << pow;
        double total_time_bintree = 0.0;
        double total_time_chain = 0.0;
        double total_time_flat = 0.0;
	double total_time_bcast=0.0;

        // Allocate data
        data = (int *)malloc(size * sizeof(int));
        if (data == NULL) {
            fprintf(stderr, "Error allocating memory for data\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        // Initialize data on root process
        if (my_rank == 0) {
            for (int j = 0; j < size; j++) {
                data[j] = j;
            }
        }

        // Execute NUM_RUNS times for each data size
        for (int i = 0; i < NUM_RUNS; i++) {
            // Barrier to synchronize processes
            MPI_Barrier(MPI_COMM_WORLD);
            start_time_bintree = MPI_Wtime(); // Start MPI timer
            binary_tree_broadcast(data, my_rank, num_procs, root_rank, size);
            end_time_bintree = MPI_Wtime(); // Stop MPI timer
            total_time_bintree += end_time_bintree - start_time_bintree;

            // Barrier to synchronize processes
            MPI_Barrier(MPI_COMM_WORLD);
            start_time_chain = MPI_Wtime(); // Start MPI timer
            chain_broadcast(data, my_rank, num_procs, root_rank, size);
            end_time_chain = MPI_Wtime(); // Stop MPI timer
            total_time_chain += end_time_chain - start_time_chain;

            // Barrier to synchronize processes
            MPI_Barrier(MPI_COMM_WORLD);
            start_time_flat = MPI_Wtime(); // Start MPI timer
            flat_tree_broadcast(data, my_rank, num_procs, root_rank, size);
            end_time_flat = MPI_Wtime(); // Stop MPI timer
            total_time_flat += end_time_flat - start_time_flat;

	    MPI_Barrier(MPI_COMM_WORLD);
	    start_time_bcast=MPI_Wtime();
	    MPI_Bcast(data,size,MPI_INT,root_rank,MPI_COMM_WORLD);
	    end_time_bcast=MPI_Wtime();
	    total_time_bcast+=end_time_bcast - start_time_bcast;

        }
        // Free allocated memory
        free(data);

        // Calculate average time 
        double avg_time_bintree = total_time_bintree / NUM_RUNS;
        double avg_time_chain = total_time_chain / NUM_RUNS;
        double avg_time_flat = total_time_flat / NUM_RUNS;
	double avg_time_bcast = total_time_bcast / NUM_RUNS;

        double max_time_bintree;
        double max_time_chain;
        double max_time_flat;
	double max_time_bcast;
        MPI_Reduce(&avg_time_bintree, &max_time_bintree, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&avg_time_chain, &max_time_chain, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&avg_time_flat, &max_time_flat, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(&avg_time_bcast, &max_time_bcast, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        // Write results to CSV file
        if (my_rank == 0) {
            fprintf(fp, "%lld,%f,%f,%f,%f\n", size, max_time_bintree * 1000, max_time_chain * 1000, max_time_flat * 1000, max_time_bcast * 1000);
        }
    }
    // Close CSV file
    fclose(fp);

    MPI_Finalize();
    return 0;
}
