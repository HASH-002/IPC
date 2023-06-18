#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main(int argc, char* argv[])
{
	int current_num_threads = 0;

	omp_set_num_threads(4);
	// Create the OpenMP parallel region, containing the number of threads as defined by OMP_NUM_THREADS
	#pragma omp parallel
	{
		// Each thread prints its identifier
		printf("Loop 1: Total threads are %d threads, This is thread %d.\n", omp_get_num_threads(), omp_get_thread_num());

		#pragma single
		{
			current_num_threads = omp_get_num_threads();
		}
	}
	return EXIT_SUCCESS;
}