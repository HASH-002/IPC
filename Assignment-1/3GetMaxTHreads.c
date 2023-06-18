#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Returns an integer that is equal to or greater than 
// the number of threads that would be available if a parallel region 
// without num_threads were defined at that point in the code.

int main(int argc, char* argv[])
{
	int current_num_threads = 0;

	// Create the OpenMP parallel region, containing the number of threads as defined by OMP_NUM_THREADS
	#pragma omp parallel 
    {
        // omp_set_num_threads(4);
        #pragma omp parallel
        {
            // omp_set_num_threads(4);
            #pragma omp single
            {
                printf("omp_get_num_threads() = %d,   omp_get_max_threads() = %d\n", omp_get_num_threads(), omp_get_max_threads());
            }
        }

        printf("\n");
        #pragma omp barrier
        #pragma omp single 
        {
            printf("omp_get_num_threads() = %d,   omp_get_max_threads() = %d", omp_get_num_threads(), omp_get_max_threads());
        }
    }
	return EXIT_SUCCESS;
}