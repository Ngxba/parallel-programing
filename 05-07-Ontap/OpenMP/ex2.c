#include <omp.h>
#include <stdio.h>

int main()
{
    int thread_nums, thread_id, cal;
    omp_set_num_threads(10);
#pragma omp parallel default(none) private(thread_id, thread_nums, cal)
    {
        thread_nums = omp_get_num_threads();
        thread_id = omp_get_thread_num();
        cal = thread_id * 10;
        printf("Num threads: %d, id thread: %d, result is: %d \n", thread_nums, thread_id, cal);
    }
}

// gcc –fopenmp  -o ex1 ex1.c
// ./ex1
