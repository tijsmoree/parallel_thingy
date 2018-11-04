
# Note that all hosts should be running exactly the same version of (Open)MPI !!

# DEBUG="-debug-daemons"
HOSTFILE=hostfile
RSHAGENT=rsh
SKIPINTFS="lo,vmnet1,vboxnet0,tap0,tap1" # Skip interfaces for e.g Virtual machines

mpirun $DEBUG \
        --hostfile $HOSTFILE \
	--mca orte_rsh_agent $RSHAGENT \
	--mca btl_tcp_if_exclude $SKIPINTFS \
	-wd $PWD $*

