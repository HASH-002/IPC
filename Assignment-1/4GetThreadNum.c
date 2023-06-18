// Returns the thread number of the thread executing within its thread team.

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int current_num_threads = 0;

    // Create the OpenMP parallel region, containing the number of threads as defined by OMP_NUM_THREADS
    omp_set_num_threads(3);
#pragma omp parallel
    {
        printf("Loop 1: Total threads are %d threads, This is thread %d.\n",
               omp_get_num_threads(), omp_get_thread_num());

#pragma single
        {
            current_num_threads = omp_get_num_threads();
        }
    }
    printf("Value of current_num_threads = %d \n", current_num_threads);

    return EXIT_SUCCESS;
}