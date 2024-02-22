#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void chain_broadcast(int *data, int my_rank, int num_procs, int root_rank, int num_elements) {
    MPI_Status status;
    int parent_rank = my_rank -  1;
    int child_rank = my_rank +  1;


    if (my_rank == root_rank) {
        MPI_Send(data, num_elements, MPI_INT, child_rank,  0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(data, num_elements, MPI_INT, parent_rank,  0, MPI_COMM_WORLD, &status);
        if (child_rank < num_procs) {
            MPI_Send(data, num_elements, MPI_INT, child_rank,  0, MPI_COMM_WORLD);
        }
    }
}

