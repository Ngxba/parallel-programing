#include <stdio.h>
#include <mpi.h>
#include <malloc.h>
#define N 20

int main(int argc, char **argv)
{
    int i;
    int *A, *B, *C;
    A = (int *)malloc(N * sizeof(int));
    B = (int *)malloc(N * sizeof(int));
    C = (int *)malloc(N * sizeof(int));
    int NP, ID, Nc, p;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &NP);
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);

    int *Ac, *Bc, *Cc;
    Ac = (int *)malloc(Nc * sizeof(int));
    Bc = (int *)malloc(Nc * sizeof(int));
    Cc = (int *)malloc(Nc * sizeof(int));
    MPI_Status Trangthai;

    // STEP 1 chia mien:
    Nc = N / NP;

    //STEP 2 khoi tao du lieu
    if (ID == 0)
    {
        for (i = 0; i < N; i++)
        {
            *(A + i) = i;
            *(B + i) = 2 * i;
        }
    }
    if (ID == 0)
    {
        for (i = 0; i < Nc; i++)
        {
            *(Ac + i) = *(A + i);
            *(Bc + i) = *(B + i);
        }

        // STEP 3:
        for (p = 1; p < NP; p++)
        {
            MPI_Send(A + Nc * p, Nc, MPI_INT, p, p, MPI_COMM_WORLD);
            MPI_Send(B + Nc * p, Nc, MPI_INT, p, 100 + p, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(Ac, Nc, MPI_INT, 0, ID, MPI_COMM_WORLD, &Trangthai);
        MPI_Recv(Bc, Nc, MPI_INT, 0, 100 + ID, MPI_COMM_WORLD, &Trangthai);
    }

    // STEP 4 tinh tong
    for (i = 0; i < Nc; i++)
    {
        *(Cc + i) = *(Ac + i) + *(Bc + i);
    }

    // STEP 5 Thu thap;
    if (ID != 0)
    {
        MPI_Send(Cc, Nc, MPI_INT, 0, ID, MPI_COMM_WORLD);
    }
    else
    {
        for (i = 0; i < Nc; i++)
        {
            *(C + i) = *(Cc + i);
        }
        for (p = 1; p < NP; p++)
        {
            MPI_Recv(C + p * Nc, Nc, MPI_INT, p, p, MPI_COMM_WORLD, &Trangthai);
        }
    }
    // STEP 6 hien thi
    if (ID == 0)
    {
        for (i = 0; i < N; i++)
            printf("%d ", *(C + i));
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
