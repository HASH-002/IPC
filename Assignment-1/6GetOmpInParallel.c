// Returns nonzero if called from within a parallel region.
#include <stdio.h>
#include <omp.h>

int main( )
{
    omp_set_num_threads(4);
    printf("outside %d\n", omp_in_parallel( ));

    #pragma omp parallel
        #pragma omp master
        {
            printf("inside %d\n", omp_in_parallel( ));
        }
}