#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int myRank, numProcs;
    int pingCount = 42;
    int pongCount = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0) {
        printf("Sending Ping (# %i)\n", pingCount);
        MPI_Send(&pingCount, 1, MPI_INT, 1, 12, MPI_COMM_WORLD);

        MPI_Recv(&pongCount, 1, MPI_INT, 1, 12, MPI_COMM_WORLD, &status);

        printf("Received Pong (# %i)\n", pongCount);
    } else {
        MPI_Recv(&pingCount, 1, MPI_INT, 0, 12, MPI_COMM_WORLD, &status);

        printf("Received Ping (# %i)\n", pingCount);

        pongCount = -pingCount;
        MPI_Send(&pongCount, 1, MPI_INT, 0, 12, MPI_COMM_WORLD);
        printf("Sending Pong (# %i)\n", pongCount);
    }

    MPI_Finalize();

    return 0;
}
