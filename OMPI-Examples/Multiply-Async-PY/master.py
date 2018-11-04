#!/usr/bin/python3

# Multiply example in Python for the MPI lab: master program.
# Jan 2017; Kees Lemmens; TWA-DIAM TU Delft.

from mpi4py import MPI
from numpy  import zeros,array
import sys

def debug(string, *vartuple ):
    print(string % (vartuple) )

def showVector(a, dim):
    xi=0
    
    for x in range(xi,dim):
        sys.stdout.write("a[%02u] = %5.2lf   " % (x , a[x]))
        if ((x+1)%4 == 0):
            print("");  # print 4 elements on one row
            
def readInput(name, a, n):
    fh = open( name, "r")
    x = 0
    
    for line in fh.readlines():
        if (x >= n ):
            break
        if (len(line) > 0):   # skip empty lines
            y = line.split()
            a[x] = float(y[0])
            x = x + 1
            
    fh.close()
    
def master(dim):
    
    S = 3
    requests = []
    
    ti1 = MPI.Wtime()
    
    comm = MPI.COMM_WORLD
    nrprocs = comm.Get_size()
    procid  = comm.Get_rank()

    if (nrprocs <= 1):
        print("Not enough slaves : probably forgot to start using mpirun ?")
        exit(1)
        
    print("Program starting with %u slaves and 1 master process ....\n" % (nrprocs - 1));
                     
    # MPI needs arrays (pointers) for messages to work properly 
    a = zeros(dim, dtype=float)
    b = zeros(dim, dtype=float)
    
    readInput("vector.in",a,dim)
    
    c = S
    m = int(dim / (nrprocs - 1))
    
    # broadcast some information to all slaves 
    c = comm.bcast(c, root = 0)
    m = comm.bcast(m, root = 0)
    
    debug("Scalar ( %03.2lf) and subvectorsize (%d) broadcasted to all slaves...",c,m)
       
    # send the subvectors
    for x in range(0,nrprocs-1):
        req = comm.Isend([a[x*m : (x+1)*m],MPI.DOUBLE],x+1,x+1)
        requests.append(req)
        
    debug("Subvectors now sent to all slaves...\n")
    showVector(a, dim)
    
    # Do anything you like here as long as you don't touch vector 'a' ...
    MPI.Request.Waitall(requests)
    
    del requests[:]  # start with an empty vector
    for x in range(0,nrprocs-1):
        req = comm.Irecv([b[x*m : (x+1)*m],MPI.DOUBLE],x+1,x+1)
        requests.append(req)

    # Do anything you like here as long as you don't touch vector 'b' ... 
    
    MPI.Request.Waitall(requests)
    
    debug("And here are the results we received : \n")
    showVector(b, dim)
    
    ti2 = MPI.Wtime()
    runtime = ti2 - ti1
    
    print("\nMaster : run time = %f secs." % runtime)
    
    return 0
