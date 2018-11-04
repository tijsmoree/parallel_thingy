
include ./ompi-config.defs

TARGET := mpihello
OBJS := $(TARGET).o
SRCS := $(OBJS:.o=.c)

LDFLAGS:=$(LDFLAGS) -s
CFLAGS:=$(CFLAGS) -I.

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): $(SRCS) Makefile

clean allclean:
	$(RM) *.o *~ core
	$(RM) -f $(TARGET)

