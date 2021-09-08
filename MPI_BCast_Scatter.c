#include <stdio.h>
#include <mpi.h>
#include <malloc.h>
#define N 10
#define M 6
#define P 8

int main (int argc, char ** argv){
    int i, j, k, p, NP, ID, MC;

    MPI_Init (&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &NP) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);
    MPI_Status stat;

    int * A, *B, *C;
    A = (int *) malloc (M*N*sizeof(int));
    B = (int *) malloc (P*N*sizeof(int));
    C = (int *) malloc (M*P*sizeof(int));
    //Step 1 -
    MC = M/NP;
    int * Ac, *Cc;
    Ac = (int *) malloc (MC*N*sizeof(int));
    Cc = (int *) malloc (MC*P*sizeof(int));

    if (ID == 0){
        //Step 2 - Khoi tao du lieu
        for (i = 0; i < M; i++)
            for (j = 0; j < N; j++)
                *(A+i*N+j) = i*N+j;
        for (j = 0; j < N; j++)
            for (k = 0; k < P; k++)
                *(B+j*P+k) = 2*(j*P+k);
    }

    //Step 3
    MPI_Scatter( A , MC*N , MPI_INT , Ac , MC*N , MPI_INT , 0 , MPI_COMM_WORLD);
    MPI_Bcast( B , N*P , MPI_INT , 0 , MPI_COMM_WORLD);

    //Step 4 Tinh tong
    for (i = 0; i < MC; i++)
        for (k = 0; k < P; k++) {
            *(Cc+i*P+k) = 0;
            for (j = 0; j < N; j++)
                *(Cc+i*P+k) = *(Cc+i*P+k) + *(Ac+i*N+j)* *(B+j*P+k);
        }

    //Step 5 - Gather
    MPI_Gather(Cc, MC*P, MPI_INT, C, MC*P, MPI_INT,0,MPI_COMM_WORLD);
    
    //Ket qua
    if(ID == 0){
        for(i = 0; i < M; i++){
            for( k = 0; k < P; k++)
                printf("%d \t", *(C+i*P+k));
            printf("\n");
        }
    }


    MPI_Finalize();
    return 0;
}
