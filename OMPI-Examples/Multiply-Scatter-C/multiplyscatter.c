/*
  Multiply example for the MPI practice: master program.
  Jan 2002; Kees Lemmens; TWA-ITS TU Delft.
  Last modified Oct 2012
*/  

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mpi.h>

//#define DEBUG 0  // print nothing
#define DEBUG 1    // only prints last 32 elements
//#define DEBUG 2  // print all elements

#define S 3  // Scalar to multiply with

/* Performs the operation : b = c . a
 * 
 * - 'a' and 'b' are two vectors with size <dim>;
 * - 'c' is a scalar;
 */

void debug(char *format,...)
{
#if (DEBUGLEVEL > 0)
   va_list p;
   
   va_start(p,format);
   vprintf(format, p);
   va_end(p);
   
#else
   ;
#endif
}

// Next function alloactes a vector of a certain size and checks for errors.
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

/* Next function simply prints a vector on the screen. */
void showVector(double *a,long dim)
{  int x, xi=0;
   
#if   (DEBUG == 0)
   xi=dim;
#elif (DEBUG == 1)
   if(dim > 32)
     xi=dim - 32;
#endif
   for(x=xi; x<dim; x++)
   {
      printf("a[%02u] = %5.2lf   ",x,a[x]);
      if ((x+1)%4 == 0)
	printf("\n");  /* print 4 elements on one row */
   }
}

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

int main(int argc, char *argv[])
{
   double *a,*b;
   double *la;  // local storage
   double c = 3;
   long dim = 16L;       // Number of elements in arrays (problemsize)

   int nrprocs;
   int procid;
   int m;        /* Size of subVector processed on one processor */
   int x;

   double ti1=0,ti2=0,runtime;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &procid);  // this processor nr
   MPI_Comm_size(MPI_COMM_WORLD, &nrprocs); // nr of processors
   
   if(argc >=2 )
     sscanf(argv[1],"%ld",(long *)&dim);
   
   if(nrprocs <= 1)
   {
      fprintf(stderr,"Not enough slaves : probably forgot to start using mpirun ?\n");
      exit(1);
   }
   
   a = initVector(dim);
   b = initVector(dim);
   
   if(procid == 0)
   {
      ti1 = MPI_Wtime(); // store starttime
   
      printf("Program starting with %u processes ....\n",nrprocs);

      readInput("vector.in",a,dim);
   }
   
   m = dim / nrprocs;
   
   la = initVector(m);
   
   // scatter the subvectors :
   MPI_Scatter(a, m, MPI_DOUBLE, la, m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   
   if(procid == 0)
   {
      showVector(a,dim);
      printf("\n");
   }
   
   /* The place where it al happens : the algorithm ;-) */
   for(x=0;x<m;x++)
     la[x] *= c;  /* multiply with the scalar */
   
   // gather the actual results in the individual subvectors :
   MPI_Gather(la, m, MPI_DOUBLE, b, m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   
   if(procid == 0)
   {
      printf("And here are the results we received : \n\n");
      showVector(b,dim);
      
      ti2 = MPI_Wtime();
      runtime = ti2 - ti1;
      
      fflush(stdout);
      printf("\nMaster : run time = %f secs.\n",runtime);
   }
   
   free(a);
   free(b);
   
   MPI_Finalize();
   return 0;
}
