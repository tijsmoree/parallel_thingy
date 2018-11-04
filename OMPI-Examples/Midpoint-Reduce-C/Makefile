
include ./ompi-config.defs

TARGETS := seqmidpointintegrate mpimidpointintegrate
OBJS    := $(TARGETS:=.o)

OPTS :=-Wall -s -O
#OPTS:=-Wall -g

CFLAGS  := -I. $(OPTS)
LDFLAGS := -lm

all : $(OBJS)
	$(foreach target, $(TARGETS), sleep 1; make $(target); )

$(OBJS) : Makefile

clean:
	$(RM) *.o *~ core

allclean: clean
	rm -vf $(TARGETS)
