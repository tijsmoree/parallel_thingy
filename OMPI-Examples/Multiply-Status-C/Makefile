
include ./ompi-config.defs

TARGET  = multiplyrnd_status

OBJSRND = main.o masterrnd.o slavernd.o
SRCSRND = $(OBJSRND:.o=.c)

LDFLAGS:=$(LDFLAGS) -s
CFLAGS:=$(CFLAGS) -I.

all: $(TARGET)

$(TARGET): $(OBJSRND)
	$(CC) -o $@ $(OBJSRND) $(LDFLAGS)

$(OBJSRND): $(SRCSRND) Makefile

clean:
	$(RM) *.o *~ core
	$(RM) -f $(TARGET)rnd
	$(RM) -rf .codelite .clang
	

allclean: clean
