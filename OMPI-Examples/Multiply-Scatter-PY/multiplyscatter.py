#!/usr/bin/python3

# Multiply Scatter/gather example in Python for the MPI lab: single program
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
    
    comm = MPI.COMM_WORLD
    nrprocs = comm.Get_size()
    procid  = comm.Get_rank()

    if (nrprocs <= 1):
        print("Not enough slaves : probably forgot to start using mpirun ?")
        exit(1)
        
    if(procid == 0):
        ti1 = MPI.Wtime()
        
        debug("Program starting with %u processes ....\n" % nrprocs)
        
        # MPI needs arrays (pointers) for messages to work properly 
        a = zeros(dim, dtype=float)
        b = zeros(dim, dtype=float)
        
        readInput("vector.in",a,dim)
    else:
        a = None
        b = None
        
    m = int(dim / nrprocs)
    
    la = zeros(m, dtype=float) # subVector
    
    # scatter the subvectors :
    comm.Scatter(a, la, root = 0)
    
    if(procid == 0):
        showVector(a, dim)
   
    for x in range(0, m):
       la[x] *= S  # The algorithm: multiply with the scalar       
       
    # gather the actual results in the individual subvectors :
    comm.Gather(la, b, root = 0)
   
    if(procid == 0):
        debug("And here are the results we received : \n")
        showVector(b,dim);
        
        ti2 = MPI.Wtime();
        runtime = ti2 - ti1
      
        print("\nMaster : run time = %f secs." % runtime)
   
    return 0

# Start the main program:
if (len(sys.argv) > 1):
    dim = int(sys.argv[1])
else:
    dim = 16
     
master(dim)
