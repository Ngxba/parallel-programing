#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>
#include <limits.h>

// Creates an array of random numbers. Each number has a value from 0 - 1
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
    if (argc != 2)
    {
        fprintf(stderr, "Usage: multiThreadSort num_elements_per_process\n");
        exit(1);
    }

    int num_elements_per_proc = atoi(argv[1]);
    // Seed the random number generator to get different results each time
    srand(time(NULL));

    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Create a random array of elements on the root process. Its total
    // size will be the number of elements per process times the number
    // of processes
    int *rand_nums = NULL;
    if (world_rank == 0)
    {
        rand_nums = create_rand_nums(num_elements_per_proc * world_size);
    }

    // For each process, create a buffer that will hold a subset of the entire
    // array
    int *sub_rand_nums = (int *)malloc(sizeof(int) * num_elements_per_proc);
    assert(sub_rand_nums != NULL);

    // Scatter the random numbers from the root process to all processes in
    // the MPI world
    MPI_Scatter(rand_nums, num_elements_per_proc, MPI_INT, sub_rand_nums,
                num_elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    bubbleSort(sub_rand_nums, num_elements_per_proc);
    // MPI_Send(sub_rand_nums,num_elements_per_proc ,MPI_INT, 0, world_rank, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    int *big_arrray = NULL;
    if (world_rank == 0)
    {
        big_arrray = (int *)malloc(sizeof(int) * world_size * num_elements_per_proc);
        assert(big_arrray != NULL);
    }

    
    MPI_Gather(sub_rand_nums, num_elements_per_proc, MPI_INT, big_arrray, num_elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
    if (world_rank == 0)
    {
        int indexOfArr[world_size];
        for (int i = 0; i < world_size * num_elements_per_proc; i++)
        {
            for (int j = 0; j < world_size; j++)
            {
                int minTemp = INT_MAX;
                int minIndex = 0;
                if (*(big_arrray + (j * num_elements_per_proc) + indexOfArr[j]) <= minTemp)
                {
                    minTemp = *(big_arrray + (j * num_elements_per_proc) + indexOfArr[j]);
                    minIndex = j;
                }
                if (j == world_size)
                {
                    indexOfArr[minIndex]++;
                    printf("%d\n", minTemp);
                }
                
            }
            
        }
        
        printArray(big_arrray, world_size * num_elements_per_proc);
        printf("%d", *(big_arrray + num_elements_per_proc));
    }

    // int *my2DArray = (int *) malloc (world_size * num_elements_per_proc * sizeof(int));
    // int *
    // if (world_rank == 0) {
    //     for (i = 1; i < world_rank; i++) {
    //         MPI_Recv(, num_elements_per_proc, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //     }
    // }

    // Clean up
    if (world_rank == 0)
    {
        free(rand_nums);
    }
    free(sub_rand_nums);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}