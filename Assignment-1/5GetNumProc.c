// Returns the number of processors 
// that are available when the function is called.

#include <stdio.h>
#include <omp.h>

int main( )
{
    omp_set_num_threads(1);
    printf("%d\n", omp_get_num_procs( ));
    #pragma omp parallel
        #pragma omp master
        {
            printf("%d\n", omp_get_num_procs( ));
        }
    
}