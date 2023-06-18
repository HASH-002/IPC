// omp_init_lock	Initializes a simple lock.
// omp_destroy_lock	Uninitializes a lock.

#include <stdio.h>
#include <omp.h>

omp_lock_t my_lock;

int main() {
    omp_init_lock(&my_lock);

    #pragma omp parallel num_threads(3)
    {
        int tid = omp_get_thread_num( );
        int i, j;

        for (i = 0; i < 5; ++i) {
            omp_set_lock(&my_lock);
            printf("Thread %d - starting locked region\n", tid);
            printf("Thread %d - ending locked region\n", tid);
            omp_unset_lock(&my_lock);
        }
    }

    omp_destroy_lock(&my_lock);
}