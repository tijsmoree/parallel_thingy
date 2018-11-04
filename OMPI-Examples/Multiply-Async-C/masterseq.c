/*
  Multiply example for the MPI practice: master program.
  Jan 2002; Kees Lemmens; TWA-DIAM TU Delft.
  Last mod. Oct 2012
 */  

#include <stdio.h>
#include <stdlib.h>
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
   double *a,*b;
   double c = S;
   
   int nrprocs;
   int m;        // Size of subVector processed on one processor
   int x;
   MPI_Request *s_req, *r_req;
   MPI_Status *s;
   double ti1,ti2,runtime;

   ti1 = MPI_Wtime(); // store starttime
   
   MPI_Comm_size(MPI_COMM_WORLD, &nrprocs); // nr of processors
   
   if(nrprocs <= 1)
   {
      fprintf(stderr,"Not enough slaves : probably forgot to start using mpirun ?\n");
      exit(1);
   }
   
   fprintf(stderr,"Program starting with %u slaves and 1 master process ....\n",nrprocs - 1);
   
   a = initVector(dim);
   b = initVector(dim);
   
   readInput("vector.in",a,dim);
      
   m = dim / (nrprocs - 1);
   
   // Allocate arrays for storing the statuses for all asynchronous communication with slaves
   s_req = (MPI_Request *)malloc(nrprocs * sizeof(MPI_Request));
   r_req = (MPI_Request *)malloc(nrprocs * sizeof(MPI_Request));
   s     = (MPI_Status  *)malloc(nrprocs * sizeof(MPI_Status));

   if(s == NULL || s_req == NULL || r_req == NULL)
   {
      fprintf(stderr,"Failed to allocate memory !!\n");
      return 1;
   }
   
   // broadcast some information to all slaves :
   MPI_Bcast(&c,        1,MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Bcast(&m,        1,MPI_INT   , 0, MPI_COMM_WORLD);

   debug("Scalar ( %03.2lf) and subvectorsize (%d) broadcasted to all slaves...\n",c,m);

   // send the subvectors :
   for(x=0; x<nrprocs-1; x++)
     MPI_Isend(&a[x*m],m,MPI_DOUBLE,x+1,x+1,MPI_COMM_WORLD,&s_req[x]);

   // Do anything you like here as long as you don't touch vector 'a' ...
 
   MPI_Waitall(nrprocs-1,s_req,s);
   // Or, a longer alternative :
   // int y;
   // for(x=0; x<nrprocs-1; x++)
   //    MPI_Waitany(nrprocs-1,s_req,&y,s);
   
   debug("Subvectors now sent to all slaves...\n\n");
   showVector(a,dim);
   debug("\n");

   for(x=0;x<nrprocs-1;x++)
   {
      fprintf(stderr,"Receiving results asynchronously: now from %d ...\n",x+1);
      // And then get the actual results :
      MPI_Irecv(&b[x*m],m,MPI_DOUBLE,x+1,x+1,MPI_COMM_WORLD,&r_req[x]);
   }
   // Do anything you like here as long as you don't touch vector 'b' ...
   MPI_Waitall(nrprocs-1,r_req,s);
   
   debug("And here are the results we received : \n\n");
   showVector(b,dim);

   ti2 = MPI_Wtime();
   runtime = ti2 - ti1;
   
   printf("\nMaster : run time = %f secs.\n",runtime);
   
   return 0;
}
