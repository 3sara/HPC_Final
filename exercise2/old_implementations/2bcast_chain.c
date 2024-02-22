#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void chain_broadcast(int *data, int my_rank, int num_procs, int root_rank) {
    MPI_Status status;
    int parent_rank = my_rank -  1;
    int child_rank = my_rank +  1;
    int received_items;

    if (my_rank == root_rank) {
        MPI_Send(data, 1, MPI_INT, child_rank,  0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(data, 1, MPI_INT, parent_rank,  0, MPI_COMM_WORLD, &status);
        if (child_rank < num_procs) {
            MPI_Send(data,1, MPI_INT, child_rank,  0, MPI_COMM_WORLD);
        }
    }
}
