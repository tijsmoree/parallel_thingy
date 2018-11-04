#!/usr/bin/python3

# MatrixSum example in Python for the MPI lab: master program.
# Feb 2017; Kees Lemmens; TWA-DIAM TU Delft.

# Performs the operation : C = A + B
# 
# - 'A' and 'B' are two square matrices with size NxN;
# - 'C' is the resulting matrix;
#
    
from mpi4py import MPI
from numpy  import zeros,array
import sys

DEBUGLEVEL = 1

def ldebug(level, string, *vartuple ):
    if (DEBUGLEVEL < level):
        return
    print(string % (vartuple) )

def initMatrix(n, m):
    mtr = zeros((m, n), dtype=float)
    return mtr
    
def fillmatrix(a, n, m, offset):
    for y in range(0, m):
        for x in range(0,n):
            a[y][x] = x + y + offset

def showMatrix(name, a, n, m):
    for y in range(0, m):
        for x in range(0, m):
            sys.stdout.write("%s[%02u][%02u] = %6.2lf   " % (name, y, x , a[y][x]))
        print("");  # print new line
        
def master(dim):
    
    info = MPI.Status()  # To store status information later
    
    ti1 = MPI.Wtime()
    
    comm = MPI.COMM_WORLD
    nrprocs = comm.Get_size()
    procid  = comm.Get_rank()

    if (nrprocs <= 1):
        print("Not enough slaves : probably forgot to start using mpirun ?")
        exit(1)
        
    print("Program starting with %u slaves and 1 master process ....\n" % (nrprocs - 1));
                     
    # MPI needs arrays (pointers) for messages to work properly 
    a = initMatrix(dim, dim)
    b = initMatrix(dim, dim)
    c = initMatrix(dim, dim)
    
#    m = array([0], dtype=int)[0]   # array with 1 element (could be 0-d array as well)

    fillmatrix(a, dim, dim, 0)
    fillmatrix(b, dim, dim, 10)
    
    m = int(dim / (nrprocs - 1))
    
    m = comm.bcast(m, root = 0)
    ldebug(0,"Subvectorsize (%d) broadcasted to all slaves...", m)
       
    # send the strips
    for y in range(1,nrprocs):
        comm.Send([a[(y-1)*m : y*m],MPI.DOUBLE],y, m * y)
        comm.Send([b[(y-1)*m : y*m],MPI.DOUBLE],y, m + y)
        
    ldebug(0,"Submatrices now sent to all slaves...\n")
    print("This is Matrix A :")
    showMatrix("A", a, dim, dim)
    print("And this is Matrix B :")
    showMatrix("B", b, dim, dim)
        
    for x in range(1,nrprocs):
        comm.Recv([c[(x-1)*m : x*m],MPI.DOUBLE],x, m * x, status = info)
        
    print("And here is the result C = A + B that we received : \n")
    showMatrix("C", c, dim, dim)
    
    ti2 = MPI.Wtime()
    runtime = ti2 - ti1
    
    print("\nMaster : run time = %f secs." % runtime)
    
    return 0
