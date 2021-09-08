#include <stdio.h>
#include <mpi.h>
#include <malloc.h>

//  BÀI TẬP LÀ ĐỂ LUỒNG 1 TẠO RA 1 MẢNG TU 1 ĐẾN 5 VÀ GỬI CHO CẢ LUỒNG 2 VÀ 3
// LUỒNG 2 SẼ TÍNH MẢNG TỪ LUỒNG 1 * 2 VÀ GỬI CHO LUONG 3
// LUONG 3 SE TINH TONG CUA 2 MANG DUOC 2 THREAD TRUYEN TOI VÀ IN RA
int main (int argc, char ** argv){
    int i, rank, size;
    // rank là luồng hiện tại, size là tổng số luồng
    MPI_Init (&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status stat;

    if (rank == 0){
        int * A;
        A = (int *) malloc (5*sizeof(int));
        for (i = 0; i < 5; i++) 
            *(A+i) = i+1;
        MPI_Send( A , 5 , MPI_INT , 1 , 1 , MPI_COMM_WORLD);
        MPI_Send( A , 5 , MPI_INT , 2 , 2 , MPI_COMM_WORLD);

    } else if (rank == 1) {
        int * B, *C;
        B = (int *) malloc (5*sizeof(int));
        C = (int *) malloc (5*sizeof(int));
        MPI_Recv( B , 5 , MPI_INT , 0 , 1 , MPI_COMM_WORLD , &stat);
        for (i = 0; i < 5; i++) 
            *(C+i) = *(B+i) * 2;

        MPI_Send( C , 5 , MPI_INT , 2 , 3 , MPI_COMM_WORLD);

    } else if (rank == 2) {
        int * D, *E, *F;
        D = (int *) malloc (5*sizeof(int));
        E = (int *) malloc (5*sizeof(int));
        F = (int *) malloc (5*sizeof(int));

        MPI_Recv( D , 5 , MPI_INT , 0 , 2 , MPI_COMM_WORLD , &stat);
        MPI_Recv(E , 5 , MPI_INT , 1 , 3 , MPI_COMM_WORLD , &stat);
    
        for (i = 0; i < 5; i++) 
            *(F+i) = *(D+i) + *(E+i);
        
        for(int i = 0; i < 5; i++){
            printf("%d \n", F[i]);
        }
    }
    
    // printf("Process %d received ", rank);
    // for(int i = 0; i < 10; i++){
    //     printf("%d ", r[i]);
    // }
    // printf("\n");

    MPI_Finalize();
    return 0;
}
