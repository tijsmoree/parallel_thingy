/*
  Matrix Sum example for the MPI practice: master program.
  June 2011; Kees Lemmens; TWA-ITS TU Delft.
  Last mod. Oct 2012

 */  

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mpi.h>

#include "main.h"

/* Performs the operation : C = A + B
 * 
 * - 'A' and 'B' are two square matrices with size NxN;
 * - 'C' is the resulting matrix;
 */

/* Next function simply allocates the rows and columns for a matrix of nxm . */
double **initMatrix(int n, int m)
{
   int t;
   double *ptr;
   double **mtr;
   
   ptr = (double  *) calloc(n * m, sizeof(double)); // rows x columns
   mtr = (double **) calloc(n, sizeof(double *));   // rows 
   
   if(ptr == NULL || mtr == NULL)
   {
      fprintf(stderr,"Malloc for matrix strip failed !\n");
      exit(1);
   }
   
   for(t=0;t<m;t++)
   {
      mtr[t] = ptr;
      ptr   += n;
   }
   
   return mtr;
}

/* Next function simply prints a matrix on the screen. */
void showMatrix(char *name, double **a, int n, int m)
{  int x,y;
 
   for(y=0; y<m; y++)
   {
      for(x=0; x<n; x++)
      {
	 fprintf(stderr,"%s[%02u][%02u]=%6.2lf  ",name,y,x,a[y][x]);
      }
      fprintf(stderr,"\n");
   }
}

/* Next function simply fills a matrix with some 'arbitrary' numbers. */
void fillMatrix(double **a, int n, int m, int offset)
{  int x,y;
   
   for(y=0; y<m; y++)
     for(x=0; x<n; x++)
       a[y][x] = (double) x+y + offset;
}

int master(int dim)
{
   double **a, **b, **c;
   int nrprocs;
   int m;        /* Height of subMatrix processed on one processor */
   int x,y;

   static MPI_Status s;
   double ti1,ti2,runtime;

   ti1 = MPI_Wtime(); // store starttime
   
   MPI_Comm_size(MPI_COMM_WORLD, &nrprocs); // nr of processors

   if(nrprocs <= 1)
   {
      printf("Not enough slaves : probably forgot to start using mpirun ?\n");
      exit(1);
   }
   
   printf("Program starting with %u slaves and 1 master process ....\n",nrprocs - 1);

   a = initMatrix(dim,dim);
   b = initMatrix(dim,dim);
   c = initMatrix(dim,dim);
   
   fillMatrix(a,dim,dim, 0);
   fillMatrix(b,dim,dim, 10);
   
   m = dim / (nrprocs - 1);

   MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
   ldebug(0,"Subvectorsize (%d) broadcasted to all slaves...\n",m);

   for(y=1; y<nrprocs; y++)
   {
      MPI_Send(&a[(y-1)*m][0],m*dim,MPI_DOUBLE,y,m * y,MPI_COMM_WORLD);
      MPI_Send(&b[(y-1)*m][0],m*dim,MPI_DOUBLE,y,m + y,MPI_COMM_WORLD);
   }
   ldebug(0,"Submatrices now sent to all slaves...\n\n");

#if (DEBUGLEVEL > 0)
   fprintf(stderr,"\nThis is Matrix A : \n");
   showMatrix("A",a,dim,dim);
   fprintf(stderr,"\nAnd this is Matrix B : \n");
   showMatrix("B",b,dim,dim);
   fprintf(stderr,"\n");
#endif

   for(x=1;x<nrprocs;x++)
   {
#ifdef USE_RANDOM_ORDER
      // Wait for a slave that is ready to transfer it's results :
      MPI_Recv(&y,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&s);
      ldebug(0,"Receiving results in random order: now from %d ...\n",y);
#else
      y=x;
      ldebug(0,"Receiving results in sequential order: now from %d ...\n",y);
#endif
      // And then get the actual results :
      MPI_Recv(&c[(y-1)*m][0],m*dim,MPI_DOUBLE,y,m * y,MPI_COMM_WORLD,&s);
   }
   
#if (DEBUGLEVEL > 0)
   fprintf(stderr,"And here is the result C = A + B that we received : \n\n");
   showMatrix("C",c,dim,dim);
#endif
   
   ti2 = MPI_Wtime();
   runtime = ti2 - ti1;
   
   fflush(stdout);
   printf("\nMaster : run time = %f secs.\n",runtime);
   
   return 0;
}
