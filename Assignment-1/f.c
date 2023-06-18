#include <omp.h>
#include <stdio.h>

#define NUM_THREADS 8
#define NUM_ITEMS 100

void compute_square(int item) {
    int result = item * item;
    printf("Thread %d computed square of %d: %d\n", omp_get_thread_num(), item, result);
}

int main() {
    int i;
    omp_set_dynamic(0);  // disable dynamic adjustment of number of threads
    omp_set_nested(1);   // enable nested parallelism

#pragma omp parallel num_threads(NUM_THREADS)
    {
        if (omp_in_parallel()) {
            printf("We are in a parallel region.\n");
        } else {
            printf("We are not in a parallel region.\n");
        }

#pragma omp for
        for (i = 0; i < NUM_ITEMS; i++) {
            compute_square(i);

#pragma omp parallel
            {
#pragma omp critical
                {
                    printf("Nested parallel region created by thread %d\n", omp_get_thread_num());
                }
            }
        }
    }

    int dynamic = omp_get_dynamic();
    if (dynamic) {
        printf("Dynamic adjustment of number of threads is enabled.\n");
    } else {
        printf("Dynamic adjustment of number of threads is disabled.\n");
    }

    int nested = omp_get_nested();
    if (nested) {
        printf("Nested parallelism is enabled.\n");
    } else {
        printf("Nested parallelism is disabled.\n");
    }

    return 0;
}