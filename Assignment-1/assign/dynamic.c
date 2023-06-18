#include <omp.h>
#include <stdio.h>

int main() {
    omp_set_dynamic(9);
    omp_set_num_threads(4);
    printf("%d\n", omp_get_dynamic());
#pragma omp parallel
    {
        // Each thread prints its identifier
        printf("Loop 1: This is thread %d and getDynamic %d.\n", omp_get_thread_num(), omp_get_dynamic());

#pragma single
        {
            int current_num_threads = omp_get_num_threads();
        }
    }
}