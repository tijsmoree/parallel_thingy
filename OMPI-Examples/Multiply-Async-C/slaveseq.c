/*
  Example for MPI practice: slave program.
  Jan. 2002; Kees Lemmens; TWA-DIAM TU Delft.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

extern void debug(char *format,...);
extern double *initVector(long dim);

int slave(void)
{
   double c;
   int m;           // subvectorSize
   int x;
   double *subVector;
   int procid;
   static MPI_Status s;
   MPI_Request req;
   
   MPI_Comm_rank(MPI_COMM_WORLD, &procid);  // this processor nr
   
   // receive some broadcast information from master :
   MPI_Bcast(&c,        1,MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Bcast(&m,        1,MPI_INT   , 0, MPI_COMM_WORLD);
   
   subVector = initVector(m);

   // get subvector from master
   MPI_Irecv(subVector,m,MPI_DOUBLE,0,procid,MPI_COMM_WORLD,&req);
   // Do anything here that doesn't require subVector ...
   MPI_Wait(&req,&s);
   
   // The place where it al happens : the algorithm ;-)
   for(x=0;x<m;x++)
     subVector[x] *= c;  // multiply with the scalar

   // and return the new values :

   debug("Slave with id %u : Now sending results ...\n",procid);

   MPI_Isend(subVector,m,MPI_DOUBLE,0,procid,MPI_COMM_WORLD,&req);
   // Do anything here that doesn't require subVector ...
   MPI_Wait(&req,&s);

   free(subVector);
   return 0;
}
