IDIR =../include
CC=mpicc

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CFLAGS=-I$(IDIR) -std=gnu99
else
CFLAGS=-I$(IDIR)
endif


ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = gyre.h cortical.h interface.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = gyre.o cortical.o interface.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gyre: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
