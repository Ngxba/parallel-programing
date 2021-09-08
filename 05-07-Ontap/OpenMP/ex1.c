#include <omp.h>
#include <stdio.h>

int main()
{
    int thread_nums, thread_id;
    omp_set_num_threads(10);
#pragma omp parallel default(none) private(thread_id, thread_nums)
    {
        thread_nums = omp_get_num_threads();
        thread_id = omp_get_thread_num();
        printf("Num threads: %d, id thread: %d\n", thread_nums, thread_id);
    }
}

// gcc –fopenmp  -o ex1 ex1.c
// ./ex1
