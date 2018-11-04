
all:  multiplyscatter

include ./ompi-config.defs

SEQOBJS := multiplyscatter.o
SEQSRCS := $(SEQOBJS:.o=.c)

LDFLAGS:=$(LDFLAGS) -s
CFLAGS:=$(CFLAGS) -I.

multiplyscatter: $(SEQSRCS) $(SEQOBJS) 
	$(CC) -o $@ $(SEQOBJS) $(LDFLAGS)

$(SEQOBJS): $(SEQSRCS) Makefile

clean:
	$(RM) *.o *~ core

allclean:
	make clean
	@$(RM) multiplyscatter
