/*  Midpoint rule : numerical integration of a given function using MPI */
/*  Kees Lemmens, Feb 2014 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

double func_val(double x)
{  //	return pow(sin(x)+cos(x),1/3.0);
   //	return sin(x);
   //	return pow(x,3)+pow(x,2)+3*x+4;
   //	return 2*pow(x,2)/ (3*pow(x,5)+4);
   	return 4/(1+x*x); // should return Pi for int from 0 .. 1 = (4 * atan(1) )
}

double simpleint(double a, int m, double h)
{  double result=0;
   int x; // stepcounter

   // This is simple midpoint integration :
   for(x=0;x<m;x++) 
     result += func_val(a + x*h + h/2); // average function value between a and a+h
   
   return result * h;                   // multiply with width=h to compute surface 
}

int main(int argc, char *argv[])
{
   int nrprocs, procid;
   double i, h, a, b, la;
   int m, lm;
   double result, lresult;
   double ti1=0, ti2=0;
   
   if(argc < 4)
   {
      fprintf(stderr,"Not enough arguments !\n");
      exit(1);
   }
   
   sscanf(argv[1],"%lf",&a);
   sscanf(argv[2],"%lf",&b);
   sscanf(argv[3],"%d",&m);

   /* Normal MPI startup */

   MPI_Init(&argc, &argv);

   MPI_Comm_size(MPI_COMM_WORLD, &nrprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &procid);
   
   printf("Process %d of %d\n", procid, nrprocs);
   
   if (procid == 0)
     ti1 = MPI_Wtime();
   
   // This part runs on all nodes (including process 0)
   i = b - a;                                // interval length
   h = i / m;                                // stepsize
   lm = m / nrprocs;                         // nr of steps per process
   if(lm == 0) lm=1;                         // ensure it is non-zero
   la = a + procid * i / nrprocs;            // local startpoint
   lresult = simpleint(la,lm,h);             // compute local result
      
   printf("Process %d : start %lf, steps %d, lresult = %lf\n",
	  procid, la, lm, lresult);

   // collect all local results and sum them into global result on process 0 :
   MPI_Reduce(&lresult, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   
   if (procid == 0)
   {
      ti2 = MPI_Wtime();
      fflush(stdout);
      fprintf(stderr,"Wall clock time = %lf\n", ti2 - ti1);
      fprintf(stderr,"The result is :  %.12lf\n",result);
   }

   MPI_Finalize();
   exit(0);
}
