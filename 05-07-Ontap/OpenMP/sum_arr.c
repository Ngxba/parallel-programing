#include <omp.h>
#include <stdio.h>
#include <malloc.h>

int main()
{
    int thread_id, sum = 0;
    int size_of_array = 4;
	int *arr = (int *)malloc((size_of_array) * sizeof(int));
    for (int i = 0; i < size_of_array; i++)
	{
		*(arr + i) = i;
	}
    omp_set_num_threads(2);
#pragma omp parallel default(none) private(thread_id) shared(arr, sum, size_of_array)
    {
        int part = size_of_array/omp_get_num_threads();
        thread_id = omp_get_thread_num();
        printf("id thread: %d\n", thread_id);
        int start = part * thread_id;
        int stop = part * (thread_id + 1);
        printf("SUM: %d\n", sum);
        printf("start: %d, stop: %d\n", start, stop);
        for (int i = start; i < stop; i++) {
            sum += *(arr+i);
        }
    }
    printf("SUM: %d\n", sum);
}

// gcc –fopenmp  -o ex1 ex1.c
// ./ex1
