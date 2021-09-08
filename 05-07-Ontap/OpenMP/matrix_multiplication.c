#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <malloc.h>
#include <time.h>
int main()
{
    srand(0);
    int num_threads, m, n, p, thread_id;
    printf("Nhap NT = ");
    scanf("%d", &num_threads);
    printf("Nhap m = ");
    scanf("%d", &m);
    printf("Nhap n = ");
    scanf("%d", &n);
    printf("Nhap p = ");
    scanf("%d", &p);
    int *A[m], *B[n], *C[m];

    for (int i = 0; i < m; i++)
    {
        A[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
            A[i][j] = rand() % 10;
        // Khoi tao ma tran C la 0 het
        // p cot, i hang
        C[i] = (int *)calloc(p, sizeof(int));
    }
    for (int i = 0; i < n; i++)
    {
        B[i] = (int *)malloc(p * sizeof(int));
        for (int j = 0; j < p; j++)
            B[i][j] = rand() % 10;
    }

    printf("A:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            printf("\t%d", A[i][j]);
        printf("\n");
    }
    printf("B:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < p; j++)
            printf("\t%d", B[i][j]);
        printf("\n");
    }

    // 1 thread cho 1 gia tri o c
    num_threads = m * p;
    omp_set_num_threads(num_threads);
#pragma omp parallel default(none) private(thread_id), shared(num_threads, A, B, C, m, n, p)
    {
        thread_id = omp_get_thread_num();
        int row = thread_id / p, col = thread_id % p;
        for (int i = 0; i < n; i++)
            C[row][col] += A[row][i] * B[i][col];
    }
    printf("A x B voi 1 thread/1 gia tri:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
            printf("\t%d", C[i][j]);
        printf("\n");
    }
    // Set C ve 0 het
    for (int i = 0; i < m; i++)
        memset(C[i], 0, p * sizeof(int));

    num_threads = p;
    omp_set_num_threads(num_threads);
#pragma omp parallel default(none) private(thread_id), shared(num_threads, A, B, C, m, n, p)
    {
        thread_id = omp_get_thread_num();
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                C[i][thread_id] += A[i][j] * B[j][thread_id];
    }
    printf("A x B voi 1 thread/1 cot:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
            printf("\t%d", C[i][j]);
        printf("\n");
    }

    for (int i = 0; i < m; i++)
        memset(C[i], 0, p * sizeof(int));
    num_threads = m;
    omp_set_num_threads(num_threads);
#pragma omp parallel default(none) private(thread_id), shared(num_threads, A, B, C, m, n, p)
    {
        thread_id = omp_get_thread_num();
        for (int i = 0; i < p; i++)
            for (int j = 0; j < n; j++)
                C[thread_id][i] += A[thread_id][j] * B[j][i];
    }
    printf("A x B voi 1 thread/1 hang:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
            printf("\t%d", C[i][j]);
        printf("\n");
    }

    int start, end;
    int Mc = m / num_threads;
    for (int i = 0; i < m; i++)
        memset(C[i], 0, p * sizeof(int));
    num_threads = num_threads;
    omp_set_num_threads(num_threads);
#pragma omp parallel default(none) private(thread_id, start, end), shared(num_threads, A, B, C, m, n, p, Mc)
    {
        int id = omp_get_thread_num();
        start = id * Mc;
        end = start + Mc;
        for (int k = start; k < end; k++)
            for (int i = 0; i < p; i++)
                for (int j = 0; j < n; j++)
                    C[k][i] += A[k][j] * B[j][i];
    }
    printf("A x B voi 1 thread/%d hang:\n", num_threads);
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
            printf("\t%d", C[i][j]);
        printf("\n");
    }
}