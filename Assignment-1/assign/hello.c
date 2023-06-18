#include <omp.h>
#include <stdio.h>

int main() {
    int nthreads = 4;
    omp_set_num_threads(nthreads);

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        printf("Hello World from thread = %d", id);
    }
}