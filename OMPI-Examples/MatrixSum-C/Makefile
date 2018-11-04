
all:  matrixsum_seq matrixsum_rnd

include ./ompi-config.defs

OBJS = master.o slave.o main.o
SRCS = $(OBJS:.o=.c)

LDFLAGS:=$(LDFLAGS) -s
CFLAGS:=$(CFLAGS) -I. -Wall

matrixsum_seq: $(SRCS) Makefile main.h
	make clean
	make CFLAGS="$(CFLAGS) -DUSE_SEQUENTIAL_ORDER" $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

matrixsum_rnd: $(SRCS) Makefile main.h
	make clean
	make CFLAGS="$(CFLAGS) -DUSE_RANDOM_ORDER" $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): $(SRCS)

clean:
	$(RM) *.o *~ core

allclean: 
	make clean
	@$(RM) matrixsum_seq matrixsum_rnd
