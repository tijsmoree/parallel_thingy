/*
  Example for MPI practice: slave program.
  June 2002; Kees Lemmens; TWA-ITS TU Delft.
  Last mod. Oct 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

#include "main.h"

int slave(int dim)
{
   int m;           /* subvectorSize */
   int x,y;
   double **subA,**subB;
   double **subC;
   int procid;
   static MPI_Status s;
   
   MPI_Comm_rank(MPI_COMM_WORLD, &procid);  // this processor nr
   
   // receive some broadcast information from master :
   MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
   
   subA = initMatrix(dim,m);
   subB = initMatrix(dim,m);
   subC = initMatrix(dim,m);

   /* get submatrices from master */
   MPI_Recv(&subA[0][0],m*dim,MPI_DOUBLE,0,m * procid,MPI_COMM_WORLD,&s);
   MPI_Recv(&subB[0][0],m*dim,MPI_DOUBLE,0,m + procid,MPI_COMM_WORLD,&s);

   // showMatrix("subA",subA,dim,m);
   // showMatrix("subB",subB,dim,m);
   
   /* The place where it al happens : the hart of the algorithm ;-) */
   for(y=0;y<m;y++)
     for(x=0;x<dim;x++)
       subC[y][x] = subA[y][x] + subB[y][x];

   // showMatrix("subC",subC,dim,m);

   ldebug(2,"Slave with id %u : Now sending results back ...\n",procid);

#ifdef USE_RANDOM_ORDER
   MPI_Send(&procid,1,MPI_INT,0,procid,MPI_COMM_WORLD);
#endif   
   MPI_Send(&subC[0][0],m*dim,MPI_DOUBLE,0,m * procid,MPI_COMM_WORLD);

   return 0;
}
