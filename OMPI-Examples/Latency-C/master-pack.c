/*
  Example for the MPI practice: master program.
  Jan 2002; Kees Lemmens; TWA-ITS TU Delft.
*/  

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <mpi.h>

int master(int argc, char *argv[])
{
   double *a,*b;
   int nrprocs;
   int x;
   int n = N, s = S;
   static MPI_Status st;
   double ti1,ti2;
   
   if(argc > 1)
     n = atoi(argv[1]);
   if(argc > 2)
     s = atoi(argv[2]);
   
   MPI_Comm_size(MPI_COMM_WORLD, &nrprocs); // nr of processors
   fprintf(stderr,"\nUsing %d slave processors for matrixsize %d, stepsize %d ...",(nrprocs -1), n, s);
   
   // Send the array size :
   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
   // Send the stepsize :
   MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
   
   a = (double *)malloc(n * sizeof(double));
   if(a == NULL) exit(1);
   
   ti1 = MPI_Wtime(); // store starttime
   
   // First send and receive the complete matrix :
   for(x=1;x<nrprocs;x++)
     MPI_Send(a,n,MPI_DOUBLE,x,x,MPI_COMM_WORLD);
   
   for(x=1;x<nrprocs;x++)
     MPI_Recv(a,n,MPI_DOUBLE,x,x,MPI_COMM_WORLD,&st);
   
   ti2 = MPI_Wtime();
   fprintf(stderr,"\nRun time complete matrix : %f secs.",ti2 - ti1);
   
   ti1 = MPI_Wtime(); // reset starttime
   
   b = (double *)malloc(n/s * sizeof(double));
   if(b == NULL) exit(1);
   
   // Now first copy the required data to a new array and then send that one single operation:
   for(x=0;x<n/s;x++)
     b[x] = a[x*s];
   for(x=1;x<nrprocs;x++)
     MPI_Send(b,n/s,MPI_DOUBLE,x,x,MPI_COMM_WORLD);
   
   // Then receive every 1/s element in a new array in a single operation and store it back:
   for(x=1;x<nrprocs;x++)
     MPI_Recv(b,n/s,MPI_DOUBLE,x,x,MPI_COMM_WORLD,&st);
   for(x=0;x<n/s;x++)
     a[x*s] = b[x];
   
   ti2 = MPI_Wtime();
   fprintf(stderr,"\nRun time 1/%d matrix : %f secs.\n",s, ti2 - ti1);
   
   return 0;
}
