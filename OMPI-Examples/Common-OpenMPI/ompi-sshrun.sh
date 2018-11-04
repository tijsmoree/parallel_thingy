
# Note that all hosts should be running exactly the same version of (Open)MPI !!

DEBUG="-debug-daemons --mca plm_base_verbose 10 "
HOSTFILE=hostfile
RSHAGENT=ssh
SKIPINTFS="lo,vmnet1,vboxnet0,tap0,tap1" # Skip interfaces for e.g Virtual machines

# PREFIX=$HOME/opt/openmpi

mpirun $DEBUG \
	$PREFIX \
	-hostfile $HOSTFILE \
	--mca orte_rsh_agent $RSHAGENT \
	--mca btl_tcp_if_exclude $SKIPINTFS \
	-wd $PWD $*
