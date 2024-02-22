#include <omp.h>
#include <stdio.h>

void openmp_bcast(int *data, int root) {
    #pragma omp parallel default(none) shared(data, root)
    {
        int tid = omp_get_thread_num();
        if (tid == root) {
            // Root thread broadcasts data to all other threads
            #pragma omp flush
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
            // Other threads wait for the data to be broadcasted
            #pragma omp flush
            *data = data[root];
        }
    }
}

int main() {
    const int num_threads =  4;
    int values[num_threads];

    #pragma omp parallel num_threads(num_threads) default(none) shared(values)
    {
        int tid = omp_get_thread_num();
        values[tid] = tid;
        printf("Thread %d has value %d\n", tid, values[tid]);

        // Call the custom broadcast function
        openmp_bcast(values,  0);

        printf("After broadcast, thread %d has value %d\n", tid, values[tid]);
    }

    return  0;
}

