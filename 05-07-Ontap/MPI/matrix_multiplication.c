#include <stdio.h>
#include <stdlib.h>
#include <mpich/mpi.h>
#include <malloc.h>

void print(int *A, int *B, int *C, int Mc, int m, int n, int p);

int main(int argc, char **argv)
{
    srand(0);

    int i, j, rank, size, Mc, m, n, p, *A, *B, *C;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status stat;


    //  Khoi tao gia tri dau vao cho CPU 0
    if (rank == 0)
    {
        printf("Nhap m = ");
        scanf("%d", &m);
        printf("Nhap n = ");
        scanf("%d", &n);
        printf("Nhap p = ");
        scanf("%d", &p);

        A = (int *) malloc(m * n * sizeof(int));
        B = (int *) malloc(n * p * sizeof(int));
        // Khởi tạo C ban đầu các giá trị = 0
        C = (int *) calloc(m * p, sizeof(int));

        // Khoi tao gia tri ngau nhien
        for (i = 0; i < m * n; i++)
            *(A + i) = rand() % 6;
        for (i = 0; i < n * p; i++)
            *(B + i) = rand() % 6;
    }

    if (m % size != 0)
    {
        fprintf(stderr, "Size need to be divisible by m\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    //  Khoi tao gia tri cho cac CPU
    MPI_Bcast(&m, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&p, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    Mc = m / size;

    if (rank != 0)
    {
        B = (int *) malloc(n * p * sizeof(int));
    }

    //  Gui B cho cac CPU
    MPI_Bcast(B, n * p, MPI_INT, 0, MPI_COMM_WORLD);

    int *Ac = (int *) malloc(Mc * n * sizeof(int));
    int *Cc = (int *) calloc(Mc * p, sizeof(int));

    // Chia nhỏ A và gửi cho các CPU, lưu vào Ac
    MPI_Scatter(A, Mc * n, MPI_INT, Ac,
                Mc * n, MPI_INT, 0, MPI_COMM_WORLD);

    //  Tinh toan
    for (i = 0; i < Mc * p; i++)
        for (j = 0; j < n; j++)
        {
            *(Cc + i) += *(Ac + n * (i / p) + j) * *(B + j * p + (i % p));
        }

    MPI_Gather(Cc, Mc * p, MPI_INT,
               C, Mc * p, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        print(A, B, C, Mc, m, n, p);
    }

    MPI_Finalize();
    return 0;
}

void print(int *A, int *B, int *C, int Mc, int m, int n, int p)
{
    printf("A:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            printf("\t%d", *(A + i * n + j));
        printf("\n");
    }
    printf("B:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < p; j++)
            printf("\t%d", *(B + i * p + j));
        printf("\n");
    }
    printf("\n===========================================\n");
    printf("\nA x B voi 1 thread la %d hang:\n", Mc);
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
            printf("\t%d", *(C + i * p + j));
        printf("\n");
    }
}