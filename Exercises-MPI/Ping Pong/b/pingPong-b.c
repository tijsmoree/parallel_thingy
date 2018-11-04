#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Maximum array size 2^10 = 1024 elements
#define MAX_ARRAY_SIZE (1<<10)

int main(int argc, char **argv) {
    int myRank, numProcs;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    srandom(MPI_Wtime() * 100000 + myRank * 137);
    int numberOfElementsToSend = random() % 100;
    int *myArray = (int *)malloc(sizeof(int) * MAX_ARRAY_SIZE);
    if (myArray == NULL) {
        printf("Not enough memory\n");
        exit(1);
    }
    int numberOfElementsReceived;

    if (myRank == 0) {
        printf("Sending %i elements\n", numberOfElementsToSend);
        MPI_Send(&numberOfElementsToSend, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(myArray, numberOfElementsToSend, MPI_INT, 1, 1, MPI_COMM_WORLD);

        MPI_Recv(&numberOfElementsReceived, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(myArray, numberOfElementsReceived, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
        printf("Received %i elements\n", numberOfElementsReceived);
    } else {
        MPI_Recv(&numberOfElementsReceived, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(myArray, numberOfElementsReceived, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        printf("Received %i elements\n", numberOfElementsReceived);

        printf("Sending back %i elements\n", numberOfElementsToSend);
        MPI_Send(&numberOfElementsToSend, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(myArray, numberOfElementsToSend, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
