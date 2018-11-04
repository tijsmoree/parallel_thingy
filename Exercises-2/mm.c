/******************************************************************************
* FILE: mm.c
* DESCRIPTION:  
*   This program calculates the product of matrix a[nra][nca] and b[nca][ncb],
*   the result is stored in matrix c[nra][ncb].
*   
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define NRA 512                 /* number of rows in matrix A */
#define NCA 512                 /* number of columns in matrix A */
#define NCB 256                 /* number of columns in matrix B */

int main (int argc, char *argv[]) 
{
int     tid, nthreads, i, j, k, chunk;
double  a[NRA][NCA],           /* matrix A to be multiplied */
        b[NCA][NCB],           /* matrix B to be multiplied */
        c[NRA][NCB];           /* result matrix C */

/* set loop iteration chunk size, experiment with several different values of chunk */
chunk = 64;

  /*** Initialize matrices ***/
  
  for (i=0; i<NRA; i++)
    for (j=0; j<NCA; j++)
      a[i][j]= i+j;
  
  for (i=0; i<NCA; i++)
    for (j=0; j<NCB; j++)
      b[i][j]= i-j;
  
  for (i=0; i<NRA; i++)
    for (j=0; j<NCB; j++)
      c[i][j]= 0;

  /* Use OpenMP to parallelize the computation of the following matrix-matrix multiplication. */
  
  for (i=0; i<NRA; i++)    
    {
    for(j=0; j<NCB; j++)       
      for (k=0; k<NCA; k++)
        c[i][j] += a[i][k] * b[k][j];
    }
  
      /* print some values of c for check. number format %lf is long float (double) */

  printf("c[0][0]=%lf, c[0][10]=%lf\n, c[10][100]=%lf, c[100][100]=%lf\n",c[0][0],c[0][10],c[10][100],c[100][100]);

}


