/*
  Deadlock example for the MPI practice: master program.
  June 2002; Kees Lemmens; ITB, Bandung
*/  

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mpi.h>

int master(int argc, char *argv[])
{
   int x,a;
   double ti1,ti2;
   int nrprocs;
   static MPI_Status s;
   
   MPI_Comm_size(MPI_COMM_WORLD, &nrprocs); // nr of processors
   ti1 = MPI_Wtime(); // store starttime

   MPI_Barrier(MPI_COMM_WORLD);
   
   // receive "ready" from slaves
   for(x=1;x<nrprocs;x++)
     MPI_Recv(&a,1,MPI_INT,x,x,MPI_COMM_WORLD,&s);
   
   ti2 = MPI_Wtime();
   printf("\nRun time : %f secs.\n",ti2 - ti1);
   
   return 0;
}
