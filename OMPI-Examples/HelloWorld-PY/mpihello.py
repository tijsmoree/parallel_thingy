#!/usr/bin/python3

from mpi4py import MPI
import sys

vmsize = MPI.COMM_WORLD.Get_size()
procid = MPI.COMM_WORLD.Get_rank()
name = MPI.Get_processor_name()

print ("Hello, I am process %d of %d on %s." % (procid, vmsize, name))
