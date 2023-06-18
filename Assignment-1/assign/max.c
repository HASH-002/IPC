#include <omp.h>
#include <stdio.h>

int main() {
    int max_threads = omp_get_max_threads();
    printf("The maximum number of threads that can be used is: %d\n", max_threads);

    return 0;
}
