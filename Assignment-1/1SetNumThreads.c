
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	int current_num_threads = 0;

	omp_set_num_threads(3);
	#pragma omp parallel
	{
		// Each thread prints its identifier
		printf("Loop 1: Total threads are %d threads, This is thread %d.\n", omp_get_num_threads(), omp_get_thread_num());

		#pragma single
		{
			current_num_threads = omp_get_num_threads();
		}
	}
    printf("\n");
    omp_set_num_threads(current_num_threads+1);

	#pragma omp parallel
	{
		printf("Loop 1: Total threads are %d threads, This is thread %d.\n", omp_get_num_threads(), omp_get_thread_num());
	}
 
	return EXIT_SUCCESS;
}