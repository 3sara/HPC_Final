#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void binary_tree_broadcast(int *data, int my_rank, int num_procs, int root_rank, int num_elements) {
    MPI_Status status;
    int parent_rank = (my_rank -  1) /  2;
    int left_child_rank =  2 * my_rank +  1;
    int right_child_rank =  2 * my_rank +  2;

    if (my_rank == root_rank) {
        if (left_child_rank < num_procs)
            MPI_Send(data,  num_elements, MPI_INT, left_child_rank,  0, MPI_COMM_WORLD);
        if (right_child_rank < num_procs)
            MPI_Send(data,  num_elements, MPI_INT, right_child_rank,  0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(data, num_elements, MPI_INT, parent_rank,  0, MPI_COMM_WORLD, &status);
        if (left_child_rank < num_procs)
            MPI_Send(data, num_elements, MPI_INT, left_child_rank,  0, MPI_COMM_WORLD);
        if (right_child_rank < num_procs)
            MPI_Send(data, num_elements, MPI_INT, right_child_rank,  0, MPI_COMM_WORLD);
    }
}

