/*
  Deadlock example for MPI practice: slave program.
  June 2002; Kees Lemmens; ITB Bandung

 Mod Nov 2008 : adapted for OpenMPI
 Mod Mar 2011 : distinguish left and right neighbour (easier for exercise)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

//#define    SIZE 256  // no deadlock on OpenMPI
#define SIZE 512       // deadlock on OpenMPI (actually already with 506)

// Note that deadlocking doesn't occur if the size of the message is very
// small due to internal buffering in the OpenMPI daemons. A size of 256
// doesn't show a deadlock, but 512 indeed does.

// Also note that we need a RING topology and NOT a pipeline or else
// deadlocks won't occur : the last process will ALWAYS be able to
// receive as it has nothing to send, so that the processor on the left can
// also deliver its message. Then this one can receive the message from it's own
// left neighbour etc. until all messages have been delivered.
 
int slave(void)
{
   int procid,nrprocs,lneighbour,rneighbour;
   static double a[SIZE],b[SIZE];
   static MPI_Status s;
   
   MPI_Comm_size(MPI_COMM_WORLD, &nrprocs); // total nr of processors
   MPI_Comm_rank(MPI_COMM_WORLD, &procid);  // this processor nr

   if(procid == 1)
     printf("Now using a blocksize of %d ...\n",SIZE);

   MPI_Barrier(MPI_COMM_WORLD);
   
   lneighbour = (procid == 1) ? nrprocs - 1 : procid - 1;
   rneighbour = (procid == nrprocs - 1) ? 1 : procid + 1; 
   
   // Send 'a' to our rneighbour if it exists :
   printf("Process %d is sending   to   %d ....\n", procid,rneighbour);
   MPI_Send(a,SIZE,MPI_DOUBLE,rneighbour,rneighbour,MPI_COMM_WORLD);
   
   // Receive 'a' from our lneighbour if it exists :
   
   // MIND THE TAG : in MPI_recv the neighbour tag here should be procid
   printf("Process %d is receiving from %d ....\n", procid,lneighbour);
   MPI_Recv(b,SIZE,MPI_DOUBLE,lneighbour,procid,MPI_COMM_WORLD,&s);
   
   // And tell the master we're ready (which might never happen ;-):
   MPI_Send(&procid,1,MPI_INT,0,procid,MPI_COMM_WORLD);
   
   return 0;
}
