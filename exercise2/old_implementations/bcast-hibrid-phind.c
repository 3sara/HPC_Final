#include <mpi.h>
#include <omp.h>
#include <stdio.h>

// Hybrid MPI and OpenMP broadcast function
void hybrid_bcast(int *data, int count, int root, MPI_Comm communicator) {
    int world_rank, world_size;
    
    // Initialize MPI if not already done
    int is_initialized;
    MPI_Initialized(&is_initialized);
    if (!is_initialized) {
        MPI_Init(NULL, NULL);
    }
    
    // Get the rank and size of the MPI communicator
    MPI_Comm_rank(communicator, &world_rank);
    MPI_Comm_size(communicator, &world_size);
    
    // MPI broadcast from root to all other processes
    if (world_rank == root) {
        for (int i =  0; i < world_size; i++) {
            if (i != world_rank) {
                MPI_Send(data, count, MPI_INT, i,  0, communicator);
            }
        }
    } else {
        MPI_Recv(data, count, MPI_INT, root,  0, communicator, MPI_STATUS_IGNORE);
    }
    
    // Spawn OpenMP threads and perform broadcast in shared memory
    #pragma omp parallel default(none) shared(data, root, count)
    {
        int tid = omp_get_thread_num();
        if (tid == root) {
            #pragma omp single
            {
                for (int i =  0; i < omp_get_num_threads(); i++) {
                    if (i != tid) {
                        #pragma omp task firstprivate(data) depend(out: data[i])
                        data[i] = data[root];
                    }
                }
            }
        } else {
            #pragma omp flush
            data[tid] = data[root];
        }
    }
    
    // Finalize MPI if initialized here
    if (!is_initialized) {
        MPI_Finalize();
    }
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Comm communicator = MPI_COMM_WORLD;
    
    // Initialize MPI
    MPI_Init(&argc, &argv);
    
    // Get the rank and size of the MPI communicator
    MPI_Comm_rank(communicator, &rank);
    MPI_Comm_size(communicator, &size);
    
    // Value to be broadcasted
    int value = rank;
    printf("Process %d has value %d\n", rank, value);
    
    // Call the hybrid broadcast function
    hybrid_bcast(&value,  1,  0, communicator);
    
    printf("After broadcast, process %d has value %d\n", rank, value);
    
    // Finalize MPI
    MPI_Finalize();
    return  0;
}

