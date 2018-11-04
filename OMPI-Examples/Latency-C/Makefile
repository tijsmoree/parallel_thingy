
include ./ompi-config.defs

LOBJS := master.o slave.o main.o
POBJS := master-pack.o slave-pack.o main.o
LSRCS := $(LOBJS:.o=.c)
PSRCS := $(POBJS:.o=.c)

LDFLAGS:=$(LDFLAGS) -s
CFLAGS:=$(CFLAGS) -I. -D N=100000 -D S=10 -g

all : latency latency-pack

latency: $(LOBJS)
	$(CC) -o $@ $(LOBJS) $(LDFLAGS)

latency-pack: $(POBJS)
	$(CC) -o $@ $(POBJS) $(LDFLAGS)

$(LOBJS): $(LSRCS) Makefile
$(POBJS): $(PSRCS) Makefile

clean:
	$(RM) *.o *~ core

allclean: clean
	rm -f latency latency-pack