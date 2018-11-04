/*
 * hello.c
 */

#include <stdio.h>
#include <omp.h>


int main()
{
    int tid;
    
#pragma omp parallel private(tid)
    {
        tid=omp_get_thread_num();
        printf("Hello from thread No %d.\n",tid);
	if (tid == 0) {
		printf("%d threads used!\n", omp_get_num_threads());
	}
    }
        return 0;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
