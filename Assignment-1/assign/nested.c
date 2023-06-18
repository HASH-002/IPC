#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 10

int main() {
    int A[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    omp_lock_t lock;
    int num_threads = 10;

    printf("Max threads = %d, Max number of processors = %d\n", omp_get_max_threads(), omp_get_num_procs());

    omp_set_num_threads(num_threads);
    omp_set_dynamic(0);
    omp_set_nested(1);
    omp_init_lock(&lock);
    printf("Number of threads requested: %d\n", num_threads);
#pragma omp parallel
    {
        // int tid = omp_get_thread_num();
        printf("thread num (inside parallel region) : %d\n", omp_get_thread_num());
        // int total_threads = omp_get_num_threads();
        printf("Number of total threads (inside parallel region): %d\n", omp_get_num_threads());

#pragma omp parallel
        {
            int id = omp_get_thread_num();
            // printf("id : %d\n",id);
            for (int i = 0; i < 10; i++) {
                omp_set_lock(&lock);
                A[i]++;
                omp_unset_lock(&lock);
            }
        }
        printf("\nIs thread %d in parallel region? -> %d\n", omp_get_thread_num(), omp_in_parallel());
    }
    printf("\nomp_get_dynamic = %d\n", omp_get_dynamic());

    printf("Number of total threads (outside the parallel region) : %d\n", omp_get_num_threads());
    printf("thread num (outside parallel region) : %d\n", omp_get_thread_num());

    printf("\n");
    omp_destroy_lock(&lock);

    for (int i = 0; i < N; i++)
        printf("%d ", A[i]);
    return 0;
}