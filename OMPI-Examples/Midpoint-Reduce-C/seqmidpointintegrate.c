/*  Midpoint rule : numerical integration of a given function */
/*  Kees Lemmens, Feb 2014 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

double func_val(double x)
{  //   return pow(sin(x)+cos(x),1/3.0); 
   //   return sin(x);
   //   return pow(x,3)+pow(x,2)+3*x+4;
   //   return 2*pow(x,2)/ (3*pow(x,5)+4);
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
{  double i, h, a, b;
   int m;
   double result;
   double ti1=0, ti2=0;

   MPI_Init(&argc,&argv); // Only used to be able to use the clock
   
   if(argc < 4)
   {
      fprintf(stderr,"Not enough arguments !\n");
      exit(1);
   }
   
   sscanf(argv[1],"%lf",&a);
   sscanf(argv[2],"%lf",&b);
   sscanf(argv[3],"%d",&m);

   printf("Sequential : start %lf, end %lf, steps %d\n", a, b, m);

   ti1 = MPI_Wtime();

   i = b - a;                                // interval length
   h = i / m;                                // stepsize

   result = simpleint(a,m,h);

   ti2 = MPI_Wtime();
   printf("Wall clock time = %lf\n", ti2 - ti1);
   printf("The result is :  %.12lf\n",result);

   MPI_Finalize();
   exit(0);
}
