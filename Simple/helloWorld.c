#include <stdio.h>
#include <mpi.h>

int main (int argc, char ** argv){
    MPI_Init (&argc, &argv);
    int NP, ID;
    MPI_Comm_size(MPI_COMM_WORLD, &NP) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);
    printf("Hi, i'm from the process %d\n", ID);
    MPI_Finalize();
    return 0;
}
