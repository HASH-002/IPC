#include <omp.h>
#include <stdio.h>

#define NUM_THREADS 4

omp_lock_t lock;
int counter = 0;

int main() {
    omp_set_num_threads(NUM_THREADS);
    omp_init_lock(&lock);

#pragma omp parallel
    {
        int i;
        for (i = 0; i < 20000; i++) {
            // omp_set_lock(&lock);
            counter++;
            // omp_unset_lock(&lock);
        }
    }

    omp_destroy_lock(&lock);
    printf("The final value of the counter is: %d\n", counter);

    return 0;
}