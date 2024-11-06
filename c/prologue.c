// begin generated source
#include <mpi.h>
#include <stdio.h>
int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int rank;
    int world;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    printf("Zero: rank %d, world: %d\n",rank, world);fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("One: rank %d, world: %d\n",rank, world);fflush(stdout);
    MPI_Finalize();
}
// end generated source
