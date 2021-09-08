#include <omp.h>
#include <stdio.h>
#include <malloc.h>

int main()
{
    int thread_id;
    int size_of_array = 20;
	int *arr = (int *)malloc((size_of_array) * sizeof(int));
    for (int i = 0; i < size_of_array; i++)
	{
		*(arr + i) = i;
	}
    omp_set_num_threads(5);
    int max = *(arr);
#pragma omp parallel default(none) private(thread_id) shared(arr, max, size_of_array)
    {
        int part = size_of_array/omp_get_num_threads();
        thread_id = omp_get_thread_num();
        printf("id thread: %d\n", thread_id);
        int start = part * thread_id;
        int stop = part * (thread_id + 1);
        printf("start: %d, stop: %d\n", start, stop);
        for (int i = start; i < stop; i++) {
            if (max < *(arr+i)){
                max = *(arr+i);
            }
        }
    }
    printf("Max: %d\n", max);
}

// gcc –fopenmp  -o ex1 ex1.c
// ./ex1
