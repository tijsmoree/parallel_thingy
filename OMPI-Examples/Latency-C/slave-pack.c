/*
  Example for MPI practice: slave program.
  Jan. 2002; Kees Lemmens; TWA-ITS TU Delft.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

int slave(void)
{
   double *a,*b;
   int procid;
   int n,s;
   int x;
   static MPI_Status st;
   
   MPI_Comm_rank(MPI_COMM_WORLD, &procid);  // this processor nr
   
   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);

   a = (double *)malloc(n * sizeof(double));
   if(a == NULL) exit(1);
   
   // Receive and send a complete matrix :
   MPI_Recv(a,n,MPI_DOUBLE,0,procid,MPI_COMM_WORLD,&st);
   MPI_Send(a,n,MPI_DOUBLE,0,procid,MPI_COMM_WORLD);

   // Receive and send only 1/s of matrix :
   
   b = (double *)malloc(n/s * sizeof(double));
   if(b == NULL) exit(1);

   MPI_Recv(b,n/s,MPI_DOUBLE,0,procid,MPI_COMM_WORLD,&st);
   for(x=0;x<n/s;x++)
     a[x*s] = b[x];
   
   for(x=0;x<n/s;x++)
     b[x] = a[x*s];
   MPI_Send(b,n/s,MPI_DOUBLE,0,procid,MPI_COMM_WORLD);
   
   return 0;
}
