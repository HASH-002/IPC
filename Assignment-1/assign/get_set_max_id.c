
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int current_num_threads = 0, max_num_threads = 0;
    omp_set_num_threads(10);
#pragma omp parallel
    {
        // Each thread prints its identifier
        printf("This is thread %d.\n", omp_get_thread_num());

#pragma single
        {
            current_num_threads = omp_get_num_threads();
            max_num_threads = omp_get_max_threads();
        }
    }
    printf("Total threads in program %d, Max threads in program %d\n", current_num_threads, max_num_threads);
    return 0;
}