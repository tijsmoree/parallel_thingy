#!/usr/bin/python3

# MatrixSum example in Python for the MPI lab: slave program.
# Feb 2017; Kees Lemmens; TWA-DIAM TU Delft.
      
from mpi4py import MPI
from numpy  import zeros,array

from master import ldebug, initMatrix, showMatrix

def slave(dim):
    
    comm = MPI.COMM_WORLD
    procid  = comm.Get_rank()
    
    m = 0
    
    # receive broadcast information from master
    m = comm.bcast(m, root = 0)
    
    subA = initMatrix(dim, m)
    subB = initMatrix(dim, m)
    subC = initMatrix(dim, m)
    
    # get submatrices from master
    comm.Recv([subA,MPI.DOUBLE],0,m * procid)
    comm.Recv([subB,MPI.DOUBLE],0,m + procid)
    
    # The place where it al happens : the algorithm ;-)
    for y in range(0, m):
        for x in range(0, dim):
            subC[y][x] = subA[y][x] + subB[y][x]
    
    ldebug(2,"Slave with id %u : Now sending results back ...",procid)
    comm.Send([subC,MPI.DOUBLE],0,m * procid)
    
    return 0
    
