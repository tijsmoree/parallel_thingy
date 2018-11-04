
include ./ompi-config.defs

TARGET  = multiply

OBJSSEQ = main.o masterseq.o slaveseq.o
OBJSRND = main.o masterrnd.o slavernd.o
SRCSSEQ = $(OBJSSEQ:.o=.c)
SRCSRND = $(OBJSRND:.o=.c)

LDFLAGS:=$(LDFLAGS) -s
CFLAGS:=$(CFLAGS) -I.

all: $(TARGET)seq $(TARGET)rnd

$(TARGET)seq: $(OBJSSEQ)
	$(CC) -o $@ $(OBJSSEQ) $(LDFLAGS)

$(TARGET)rnd: $(OBJSRND)
	$(CC) -o $@ $(OBJSRND) $(LDFLAGS)

$(OBJSSEQ): $(SRCSSEQ) Makefile

$(OBJSRND): $(SRCSRND) Makefile

clean:
	$(RM) *.o *~ core
	$(RM) -f $(TARGET)rnd $(TARGET)seq
	$(RM) -rf .codelite .clang
	

allclean: clean
