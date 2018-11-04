
include ./ompi-config.defs

TARGET := deadlock
OBJS   := main.o master.o slave.o
SRCS   := $(OBJS:.o=.c)

LDFLAGS:=$(LDFLAGS) -s
CFLAGS:=$(CFLAGS) -I.

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): $(SRCS) Makefile

clean:
	$(RM) *.o *~ core

allclean:
	make clean
	rm -f $(TARGET)
