#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

// Creates an array of random numbers. Each number has a value from 0 - 100
int *create_rand_nums(int num_elements)
{
    int *rand_nums = (int *)malloc(sizeof(int) * num_elements);
    assert(rand_nums != NULL);
    int i;
    for (i = 0; i < num_elements; i++)
    {
        rand_nums[i] = (rand() % 100);
    }
    return rand_nums;
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(int *arr, int num_elements)
{
    int i, j;
    for (i = 0; i < num_elements - 1; i++)
        // Last i elements are already in place
        for (j = 0; j < num_elements - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

/* Function to print an array */
void printArray(int *arr, int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main(int argc, char **argv)
{
    int num_elements = atoi(argv[0]);
    srand(time(NULL));
    clock_t begin, end;
    begin = clock();
    int *rand_nums = NULL;
    rand_nums = create_rand_nums(num_elements);
    bubbleSort(rand_nums, num_elements);
    printArray(rand_nums, num_elements);
    end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent: %.5fs \n", time_spent);
}