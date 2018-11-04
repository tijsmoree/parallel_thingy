#!/usr/bin/python3
# Kees Lemmens, Diam TUDelft, MPI course, Feb 2014
"""
usage::

  $ mpiexec -n <nprocs> python midpointintegrate.py
"""

from mpi4py import MPI
from math   import pi as PI
from numpy  import array
import sys

def func_val(x):
    # return pow(sin(x)+cos(x),1/3.0)
    # return sin(x)
    # return pow(x,3)+pow(x,2)+3*x+4;
    # return 2*pow(x,2)/ (3*pow(x,5)+4);
    return 4/(1+x*x) # should return Pi for int from 0 .. 1 = (4 * atan(1) )

def simpleint(a, m, h):
    result = 0
    
    # This is simple midpoint integration
    for x in range(0,m):
        result += func_val(a + x*h + h/2) # average function value between a and a+h
            
    return result * h               # multiply with width=h to compute surface

def main():
    
    if(len(sys.argv) < 4):
        print("Not enough arguments !")
        exit(1)
        
    a = float(sys.argv[1])
    b = float(sys.argv[2])
    m = int(sys.argv[3])
    
    ti1 = MPI.Wtime()
        
    result = 0 

    i = float(b-a)                            # interval length (force float to avoid roundoff errors)
    h = i / m                                 # stepsize

    result = simpleint(a, m, h)
                  
    ti2 = MPI.Wtime()
    print("Wall clock time = %lf" % (ti2 - ti1))
    print("The result is :  %.12lf" % result)
       
main()
