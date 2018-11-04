/******************************************************************************
* Example - Matrix-vector multiplication - C/C++ Version
* FILE: MatrixVector.c
* DESCRIPTION:
*   This example multiplies matrix A with a vector element and 
*   stores the summed products in vector c.  
******************************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#define SIZE 1000


int main()
{

   float A[SIZE][SIZE], b[SIZE], c[SIZE];
   int i, j;
   double fTimeStart, fTimeEnd;

   /* Initializations */
   for (i=0; i < SIZE; i++)
   {
     for (j=0; j < SIZE; j++)
         /* fminf(x,y) gives the minimum of x and y  */
       A[i][j] = fminf(i*1.0/(j+1.0),j*1.0/(i+1.0));
     b[i] = 1.0 * (i+1);
     c[i] = 0.0;
   }

   fTimeStart = omp_get_wtime();
#pragma omp parallel for private(i, j)
   for (j=0; j < SIZE; j++)
     for (i=0; i < SIZE; i++)
       c[i] = c[i] + A[j][i] * b[i];


   fTimeEnd = omp_get_wtime();
   printf("  wall clock time     = %.20f\n", fTimeEnd - fTimeStart);
   
   return 0;
}
