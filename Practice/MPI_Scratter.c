#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){

    int process_Rank, size_Of_Comm;
    int distro_Array[4] = {39, 72, 129, 42};
    int scattered_Data;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Comm);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    MPI_Scatter(
        &distro_Array,      //Address of array we are scattering from.
        1,                  //Number of items we are sending each processor
        MPI_INT,            //MPI Datatype of scattering array.
        &scattered_Data,    //Address of array we are receiving scattered data.
        1,                  //Amount of data each process will receive.
        MPI_INT,            //MPI Datatype of receiver array.
        0,                  //Process ID that will distribute the data.
        MPI_COMM_WORLD      //MPI Communicator.
    );

    printf("Process %d has received: %d \n", process_Rank, scattered_Data);

    MPI_Finalize();
    return 0;
}