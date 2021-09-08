#include <stdio.h>
#include <mpich/mpi.h>
#include <malloc.h>

#define ARR_SIZE 10

int main(int argc, char **argv)
{
    int rank, size;
    int *array_1, *array_2, *sub_array_1, *sub_array_2, *result, sub_arr_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        array_1 = (int *) malloc(ARR_SIZE * sizeof(int));
        array_2 = (int *) malloc(ARR_SIZE * sizeof(int));
        result = (int *) malloc(ARR_SIZE * sizeof(int));

        for (int i = 0; i < ARR_SIZE; ++i)
        {
            array_1[i] = i;
            array_2[i] = i * 2;
        }
    }

    sub_arr_size = ARR_SIZE / size;

    sub_array_1 = (int *) malloc(sub_arr_size * sizeof(int));
    sub_array_2 = (int *) malloc(sub_arr_size * sizeof(int));

    MPI_Scatter(array_1, sub_arr_size, MPI_INT,
                sub_array_1, sub_arr_size, MPI_INT,
                0, MPI_COMM_WORLD);

    MPI_Scatter(array_2, sub_arr_size, MPI_INT,
                sub_array_2, sub_arr_size, MPI_INT,
                0, MPI_COMM_WORLD);

    for (int i = 0; i < sub_arr_size; ++i)
    {
        sub_array_1[i] += sub_array_2[i];
    }

    MPI_Gather(sub_array_1, sub_arr_size, MPI_INT, result, sub_arr_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 0; i < ARR_SIZE; ++i)
        {
            printf("%d ", array_1[i]);
        }
        printf("\n");
        for (int i = 0; i < ARR_SIZE; ++i)
        {
            printf("%d ", array_2[i]);
        }
        printf("\n");
        for (int i = 0; i < ARR_SIZE; ++i)
        {
            printf("%d ", result[i]);
        }
    }

    MPI_Finalize();
    return 0;
}