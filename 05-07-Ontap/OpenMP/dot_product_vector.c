#include <omp.h>
#include <stdio.h>
#include <malloc.h>

int main()
{
    int thread_id;
    int size_of_array = 10;
    int *A, *B, *C;
	A = (int *)malloc((size_of_array) * sizeof(int));
    B = (int *)malloc((size_of_array) * sizeof(int));
    C = (int *)malloc((size_of_array) * sizeof(int));
    for (int i = 0; i < size_of_array; i++)
	{
		*(A + i) = i;
        *(B + i) = i*2;
	}
    omp_set_num_threads(2);
#pragma omp parallel default(none) private(thread_id) shared(A, B, C, size_of_array)
    {
        int part = size_of_array/omp_get_num_threads();
        thread_id = omp_get_thread_num();
        printf("id thread: %d\n", thread_id);
        int start = part * thread_id;
        int stop = part * (thread_id + 1);
        printf("start: %d, stop: %d\n", start, stop);
        for (int i = start; i < stop; i++) {
            *(C+i) = *(A+i) * *(B+i);
        }
    }
    for (int i = 0; i < size_of_array; i++)
	{
		printf("A[%d]:%d", i, *(A + i));
		printf("\tB[%d]:%d", i, *(B + i));
		printf("\tC[%d]:%d\n", i, *(C + i));
	}
}

// gcc –fopenmp  -o ex1 ex1.c
// ./ex1
