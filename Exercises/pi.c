/*
 *  pi.c calculates the value of Pi through numerical integration of
 *  the function f(x)=4/(1+x*x) over [0,1]. The interval [0,1] is divided
 *  into n subintervals each of size h=1/n.
 *
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>


double CalcPi(int n);

int main(int argc, char **argv)
{
    int n = 150000000;
    const double fPi25DT = 3.141592653589793238462643;
    double fPi;
    double fTimeStart, fTimeEnd;
    
#ifdef READ_INPUT  
    printf("Enter the number of intervals: ");
    scanf("%d",&n);
#endif

    if (n <= 0 || n > 2147483647 ) 
    {
        printf("\ngiven value has to be between 0 and 2147483647\n");
        return 1;
    }
    /*  omp_get_wtime gets the wall time */
    fTimeStart = omp_get_wtime();

    /* the calculation is done here*/
    fPi = CalcPi(n);

    fTimeEnd = omp_get_wtime();
    printf("\npi is approximately = %.20f \nError               = %.20f\n",
           fPi, fabs(fPi - fPi25DT));
    printf("  wall clock time     = %.20f\n", fTimeEnd - fTimeStart);

    return 0;
}


double f(double a)
{
    return (4.0 / (1.0 + a*a));
}


double CalcPi(int n)
{
    const double fH   = 1.0 / (double) n;
    double fSum = 0.0;
    double fX;
    int i;

    #pragma omp parallel for private(i, fX) reduction(+:fSum)
    for (i = 0; i < n; i += 1)
    {
        fX = f(fH * ((double)i + 0.5));
	
        fSum += fX;
    }
    return fH * fSum;
}
