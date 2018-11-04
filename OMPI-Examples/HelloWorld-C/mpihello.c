#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int main(int argc, char *argv[])
{
  int procid,vmsize;
  int exitstatus=0;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &vmsize);  // get number of nodes
  MPI_Comm_rank(MPI_COMM_WORLD, &procid);  // get our process nr
  
  printf("Node %d of %d says: Hello World !\n", procid, vmsize);
  
  MPI_Finalize();
  exit(exitstatus);
}
