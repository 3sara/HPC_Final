#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void flat_tree_broadcast(int *data, int my_rank, int num_procs, int root_rank, int  num_elements) {
    MPI_Status status;

    if (my_rank == root_rank) {
            for (int i=1; i<num_procs;i++){
	    MPI_Send(data,  num_elements , MPI_INT, i,  0, MPI_COMM_WORLD);
    }}
    else {
        MPI_Recv(data,   num_elements , MPI_INT, root_rank,  0, MPI_COMM_WORLD, &status);
    }
}
