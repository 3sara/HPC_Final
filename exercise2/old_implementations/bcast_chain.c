#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void chain_broadcast(int *data, int my_rank, int num_procs, int root_rank) {
    MPI_Status status;
    int parent_rank = my_rank -  1;
    int child_rank = my_rank +  1;
    int received_items;

    if (my_rank == root_rank) {
        printf("Process %d has data %d\n", my_rank, *data);
        MPI_Send(data,  1, MPI_INT, child_rank,  0, MPI_COMM_WORLD);
    } else {
        printf("Process %d waiting for data\n", my_rank);
        MPI_Recv(data,  1, MPI_INT, parent_rank,  0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &received_items);
        if (received_items >  0) {
            printf("Process %d received data %d from process %d\n", my_rank, *data, parent_rank);
        } else {
            printf("No data received by process %d\n", my_rank);
        }
        if (child_rank < num_procs) {
            MPI_Send(data,  1, MPI_INT, child_rank,  0, MPI_COMM_WORLD);
        }
    }
}

