#!/usr/bin/python3

# Multiply example in Python for the MPI lab: slave program.
# Jan 2017; Kees Lemmens; TWA-DIAM TU Delft.
      
from mpi4py import MPI
from numpy  import zeros,array

def debug(string, *vartuple ):
    print(string % (vartuple) )

def slave():
    
    comm = MPI.COMM_WORLD
    procid  = comm.Get_rank()
    
    c = float(0.0)
    m = int(0)
    
    # receive some broadcast information from master
    c = comm.bcast(c, root = 0)
    m = comm.bcast(m, root = 0)
    
    subVector = zeros(m, dtype=float)
    
    # get subvector from master
    req = comm.Irecv([subVector,MPI.DOUBLE],0,procid)
    
    # Do anything here that doesn't require subVector ...
    req.Wait()
    
    # The place where it al happens : the algorithm ;-)
    for x in range(0, m):
        subVector[x] *= c  # multiply with the scalar
    
    # and return the new values
    debug("Slave with id %u : Now sending results ...",procid)
    req = comm.Isend([subVector,MPI.DOUBLE],0,procid)
    
    # Do anything here that doesn't require subVector ...
    req.Wait()
    
    return 0
    
