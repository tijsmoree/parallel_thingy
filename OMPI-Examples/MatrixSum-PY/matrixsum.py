#!/usr/bin/python3

# Generic example in Python for the MPI lab: main program.
# Feb 2014; Kees Lemmens; TWA-DIAM TU Delft.

from mpi4py import MPI
from numpy  import array
import sys

from master import master
from slave  import slave

def main():
    
    comm = MPI.COMM_WORLD
    procid  = comm.Get_rank()
    dim = 4

    if (len(sys.argv) > 1):
       dim = int(sys.argv[1])
       
    if procid == 0:
        print ("Starting master on %d ..." % ( procid ))
        master(dim)
    else:
        print ("Starting slave on %d ..." % ( procid ))
        slave(dim)

    return 0

main()
