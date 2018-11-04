#!/usr/bin/python3

# Generic example in Python for the MPI lab: main program.
# Jan 2017; Kees Lemmens; TWA-DIAM TU Delft.

from mpi4py import MPI
from numpy  import array
import sys

from master import master
from slave  import slave

def main():
    
    comm = MPI.COMM_WORLD
    procid  = comm.Get_rank()
    dim = 16

    if (len(sys.argv) > 1):
       dim = int(sys.argv[1])
       
    if procid == 0:
        print ("Starting master for async communication on %d ..." % ( procid ))
        master(dim)
    else:
        print ("Starting slave for async communication on %d ..." % ( procid ))
        slave()

    return 0

main()
