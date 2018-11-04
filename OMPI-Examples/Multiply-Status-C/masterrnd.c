/*
  Multiply example using status info for receiving results in random order: master program.
  Feb 2017; Kees Lemmens; TWA-DIAM TUDelft.
  Last mod. Feb 2017
*/  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // memcpy
#include <time.h>

#include <mpi.h>

//#define DEBUG 0  // print nothing
#define DEBUG 1    // only prints last 32 elements
//#define DEBUG 2  // print all elements

#define S 3  // Scalar to multiply with

extern void debug(char *format,...);

/* Performs the operation : b = c . a
 * 
 * - 'a' and 'b' are two vectors with size <dim>;
 * - 'c' is a scalar;
 */

// Next function allocates a vector of a certain size and checks for errors.
double *initVector(long dim)
{
   double *ptr;
   
   ptr = (double  *) malloc(dim * sizeof(double));
   
   if (ptr == NULL)
   {
      fprintf(stderr,"Malloc for vector failed !\n");
      exit(1);
   }
   
   return ptr;
}

// Next function simply prints a vector on the screen.
void showVector(double *a, long dim)
{  int x, xi=0;
   
#if   (DEBUG == 0)
   xi=dim;
#elif (DEBUG == 1)
   if(dim > 32)
     xi=dim - 32;
#endif
 
   for(x=xi; x<dim; x++)
   {
      fprintf(stderr,"a[%02u] = %5.2lf   ",x,a[x]);
      if ((x+1)%4 == 0)
	fprintf(stderr,"\n");  // print 4 elements on one row
   }
}

// This reads the data from an inputfile into array a
int readInput(char *name,double *a,int n)
{
   FILE *in;
   char buffer[100];
   int x=0;
   
   if((in = fopen(name,"r")) == NULL) 
   {
      fprintf(stderr,"Open of inputfile %s failed !!\n",name);
      return 1;
   }

   for(x=0;x<n;x++)
   {
      if(fgets(buffer,100,in) != NULL)
	a[x] = atof(buffer);
      else
      {
	 fprintf(stderr,"Inputfile %s too short !!\n",name);
	 return 1;
      }
   }
   fclose(in);
   return 0;
}

int master(long dim)
{
   double *a,*b,*t;
   double c = S;
      
   int nrprocs;
   int m;        // Size of subVector processed on one processor
   int x,y;

   static MPI_Status s;
   double ti1,ti2,runtime;

   ti1 = MPI_Wtime(); // store starttime
   
   MPI_Comm_size(MPI_COMM_WORLD, &nrprocs); // nr of processors
   
   if(nrprocs <= 1)
   {
      fprintf(stderr,"Not enough slaves : probably forgot to start using mpirun ?\n");
      exit(1);
   }
      
   a = initVector(dim);
   b = initVector(dim);
   
   fprintf(stderr,"Program starting with %u slaves and 1 master process ....\n",nrprocs - 1);

   readInput("vector.in",a,dim);
   
   m = dim / (nrprocs - 1);

   // broadcast some information to all slaves :
   MPI_Bcast(&c,        1,MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Bcast(&m,        1,MPI_INT   , 0, MPI_COMM_WORLD);

   debug("Scalar ( %03.2lf) and subvectorsize (%d) broadcasted to all slaves...\n",c,m);

   // send the subvectors :
   for(x=0; x<nrprocs-1; x++)
     MPI_Send(&a[x*m],m,MPI_DOUBLE,x+1,x+1,MPI_COMM_WORLD);

   debug("Subvectors now sent to all slaves...\n\n");
   showVector(a,dim);
   debug("\n");

   t = initVector(m); // temporary buffer

   for(x=0;x<nrprocs-1;x++)
   {
      // And then get the actual results :
      MPI_Recv(t,m,MPI_DOUBLE,MPI_ANY_SOURCE,12345,MPI_COMM_WORLD,&s);
      y = s.MPI_SOURCE;  // get the sender id from status
      
      fprintf(stderr,"Received result in random order: now from %d ...\n",y);
      showVector(t,m);      
      memcpy(&b[(y-1)*m], t, m * sizeof(double)); // and copy to correct location in b
   }
   
   debug("And here are the results we received : \n\n");
   showVector(b,dim);

   ti2 = MPI_Wtime();
   runtime = ti2 - ti1;
   
   printf("\nMaster : run time = %f secs.\n",runtime);
   
   return 0;
}
