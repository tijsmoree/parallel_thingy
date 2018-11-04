#!/usr/bin/python3

# Multiply example in Python for the MPI lab: slave program.
# Feb 2014; Kees Lemmens; TWA-DIAM TU Delft.
      
from mpi4py import MPI
from numpy  import zeros,array

from master import debug

def slave():
    
    comm = MPI.COMM_WORLD
    procid  = comm.Get_rank()
    
    c = float(0.1)
    m = int(0)
    
    # receive some broadcast information from master
    c = comm.bcast(c, root = 0)
    m = comm.bcast(m, root = 0)

    subVector = zeros(m, dtype=float)
    
    # get subvector from master
    comm.Recv([subVector,MPI.DOUBLE],0,procid)
    
    # The place where it al happens : the algorithm ;-)
    for x in range(0, m):
        subVector[x] *= c  # multiply with the scalar
    
    # and return the new values
    debug("Slave with id %u : Now sending results ...",procid)
    comm.Send([subVector,MPI.DOUBLE],0,procid)
        
    return 0
    
