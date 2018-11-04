
include ./ompi-config.defs

TARGET  := multiply

OBJSSEQ := main.o masterseq.o slaveseq.o
SRCSSEQ := $(OBJSSEQ:.o=.c)

LDFLAGS:=$(LDFLAGS) -s
CFLAGS:=$(CFLAGS) -I.

all: $(TARGET)seq

$(TARGET)seq: $(OBJSSEQ)
	$(CC) -o $@ $(OBJSSEQ) $(LDFLAGS)

$(OBJSSEQ): $(SRCSSEQ) Makefile

clean:
	$(RM) *.o *~ core
	$(RM) -f $(TARGET)seq

allclean: clean