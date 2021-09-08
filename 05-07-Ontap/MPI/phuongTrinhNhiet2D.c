#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <mpich/mpi.h>

#define m 200
#define n 200

#define T 60 // Calculate time (m)
#define dt 0.01
#define dx 0.1
#define D 0.1 // Heat transfer coefficient
#define HEAT_CENTER_SIZE 50

//=========================
void Write2File(float *C[], char *c)
{
    FILE *result = fopen("result2.txt", c);
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            fprintf(result, "%lf\t", *(C + i * n + j));
        fprintf(result, "\n");
    }
    fclose(result);
}

//=========================
void KhoiTao(float *C[])
{
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
        {
            // Set heat in center area (center node - 50 to center node + 50 -- both vertical and horizontal) to 80
            if (i >= (m / 2 - HEAT_CENTER_SIZE) && i < (m / 2 + HEAT_CENTER_SIZE) && j >= (n / 2 - HEAT_CENTER_SIZE) &&
                j < (n / 2 + HEAT_CENTER_SIZE))
                C[i][j] = 80.0;
            else
                // Set other nodes to 25
                C[i][j] = 25.0;
        }
}

//=========================
void FD(float *C[], float *dC[], float *Cu, float *Cd, int Mc)
{
    float current, up, down, left, right;
    for (int i = 0; i < Mc; i++)
        for (int j = 0; j < n; j++)
        {
            current = C[i][j];
            up = (i == 0) ? Cu[j] : C[i - 1][j];
            down = (i == Mc - 1) ? Cd[j] : C[i + 1][j];
            left = (j == 0) ? 25 : C[i][j - 1];
            right = (j == n - 1) ? 25 : C[i][j + 1];

            dC[i][j] = D * (up + down + left + right - 4 * current) / (dx * dx);
        }
}

//=========================
int main(int argc, char **argv)
{
    int i, j, rank, size, Ntime = T / dt, Mc;
    float t = 0, *C[m], *dC[m], *Cu, *Cd;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status stat;

    //  Initialize C and dC at rank 0
    if (rank == 0)
    {
        // Initialize C
        for (i = 0; i < m; i++)
            C[i] = (float *) malloc(n * sizeof(float));

        // Initialize dC
        for (i = 0; i < m; i++)
            dC[i] = (float *) calloc(n, sizeof(float));

        KhoiTao(C);
        // Write2File(C, "w");
    }

    //  Khoi tao gia tri cho cac CPU
    Mc = m / size;

    float *Cs[Mc], *dCs[Mc];
    for (i = 0; i < Mc; i++)
        Cs[i] = (float *) malloc(n * sizeof(float));
    for (i = 0; i < Mc; i++)
        dCs[i] = (float *) malloc(n * sizeof(float));

    Cu = (float *) malloc(n * sizeof(float));
    Cd = (float *) malloc(n * sizeof(float));

    //  Gui input cho cac CPU
    MPI_Scatter(C, Mc, MPI_FLOAT, Cs, Mc, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // MPI_Scatter(dC, Mc * n, MPI_FLOAT, dCs, Mc * n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    //  Vong lap tinh toan Ntime lan
    time_t t1, t2;
    t1 = time(NULL);
    for (t = 0; t < Ntime; t++)
    {
        //  Broadcast Cu
        if (rank == 0) // First Mc
        {
            for (i = 0; i < n; i++)
                Cu[i] = 25;
            MPI_Send(Cs[Mc - 1], n, MPI_FLOAT, rank + 1, rank, MPI_COMM_WORLD);
        }
        else if (rank == size - 1) // Last Mc
            MPI_Recv(Cu, n, MPI_FLOAT, rank - 1, rank - 1, MPI_COMM_WORLD, &stat);
        else // Mc at the middle
        {
            MPI_Send(Cs[Mc - 1], n, MPI_FLOAT, rank + 1, rank, MPI_COMM_WORLD);
            MPI_Recv(Cu, n, MPI_FLOAT, rank - 1, rank - 1, MPI_COMM_WORLD, &stat);
        }

        //  Broadcast Cd
        if (rank == 0)
            MPI_Recv(Cd, n, MPI_FLOAT, rank + 1, rank + 1, MPI_COMM_WORLD, &stat);
        else if (rank == size - 1)
        {
            for (i = 0; i < n; i++)
                Cd[i] = 25;
            MPI_Send(Cs[0], n, MPI_FLOAT, rank - 1, rank, MPI_COMM_WORLD);
        }
        else
        {
            MPI_Send(Cs[0], n, MPI_FLOAT, rank - 1, rank, MPI_COMM_WORLD);
            MPI_Recv(Cd, n, MPI_FLOAT, rank + 1, rank + 1, MPI_COMM_WORLD, &stat);
        }

        //  Tinh toan dao ham
        FD(Cs, dCs, Cu, Cd, Mc);

        //  Cap nhat Cs
        for (i = 0; i < Mc; i++)
        {
            for (int j = 0; j < n; ++j)
            {
                Cs[i][j] += dt * dCs[i][j];
            }
        }

        // Synchronize other threads (force all threads to wait at this step)
        MPI_Barrier(MPI_COMM_WORLD);
    }

    // Nhan output tu cac CPU
    MPI_Gather(Cs, Mc * n, MPI_FLOAT, C, Mc * n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        t2 = time(NULL);
        printf("\tThe Calculation time: %ld\n", (long) (t2 - t1));
        Write2File(C, "w");
    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}