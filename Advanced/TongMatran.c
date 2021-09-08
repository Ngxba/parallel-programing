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
    int * Ac, *Bc, *Cc;
    Ac = (int *) malloc (MC*N*sizeof(int));
    Bc = (int *) malloc (N*P*sizeof(int));
    Cc = (int *) malloc (MC*P*sizeof(int));

    if (ID == 0){
        //Step 2 - Khoi tao du lieu
        for (i = 0; i < M; i++)
            for (j = 0; j < N; j++) 
                *(A+i*N+j) = i*N+j;
        for (j = 0; j < N; j++)
            for (k = 0; k < P; k++) 
                *(B+j*P+k) = 2*(j*P+k);
                
    //Step 3
        for (i = 0; i < MC; i++) 
            for (j = 0; j < N; j++) 
                *(Ac+i*N+j) = *(A+i*N+j);
        for (j = 0; j < N; j++)
            for (k = 0; k < P; k++)  
                *(Bc+j*P+k) = *(B+j*P+k);
        

        for (p = 1; p < NP; p++) {
            MPI_Send(A + MC*N*p, MC*N, MPI_INT, p, p, MPI_COMM_WORLD);
            MPI_Send(B, N*P, MPI_INT, p, 100000+p, MPI_COMM_WORLD);
        }
    
    } else {
        MPI_Recv( Ac , MC*N , MPI_INT , 0 , ID , MPI_COMM_WORLD , &stat);
        MPI_Recv( Bc , N*P , MPI_INT , 0 , 100000+ID , MPI_COMM_WORLD , &stat);
    }

    //Step 4 Tinh tong
    for (i = 0; i < MC; i++) 
        for (k = 0; k < P; k++) {
            *(Cc+i*P+k) = 0;
            for (j = 0; j < N; j++)
                *(Cc+i*P+k) = *(Cc+i*P+k) + *(Ac+i*N+j)* *(Bc+j*P+k);
        }

    //Step 5 - Gather
    if(ID != 0){
        MPI_Send(Cc, MC*P, MPI_INT, 0, ID, MPI_COMM_WORLD);
    } else {
        for (i = 0; i < MC; i++) 
            for (k = 0; k < P; k++) 
                *(C+i*P+k) = *(Cc+i*P+k);
        for (p = 1; p < NP; p++) 
            MPI_Recv( C+p*MC*P , MC*P , MPI_INT , p , p , MPI_COMM_WORLD , &stat);
    }
        
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
