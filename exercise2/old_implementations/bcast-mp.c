#include <mpi.h>
#include <omp.h>
#include <stdio.h>

void my_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {
    
    #pragma omp parallel
    //int my_thread_id = omp_get_thread_num();
    	int world_rank;
    MPI_Comm_rank(communicator, &world_rank);
    int world_size;
    MPI_Comm_size(communicator, &world_size);
    #pragma omp master
        // Root process sends data to all other processes
    	    for (int i =  1; i < world_size; i++) {
            
                MPI_Send(data, count, datatype, i,  0, communicator);
	    }       
        // Other processes receive data from the root
        MPI_Recv(data, count, datatype, root,  0, communicator, MPI_STATUS_IGNORE);
    }




int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int value = rank;
    printf("Process %d has value %d\n", rank, value);

    // Call the custom broadcast function
    my_bcast(&value,  1, MPI_INT,  0, MPI_COMM_WORLD);

    printf("After broadcast, process %d has value %d\n", rank, value);

    MPI_Finalize();
    return  0;
}

