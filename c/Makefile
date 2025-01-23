IDIR =../include
CC=gcc
MPICC=mpicc

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CFLAGS=-I$(IDIR) -std=gnu99
else
CFLAGS=-I$(IDIR)
endif


ODIR=obj
LDIR =../lib

LIBS=-lm

_SODEPS = cortical.h interface.h
SODEPS = $(patsubst %,$(IDIR)/%,$(_SODEPS))

_GDEPS = gyre.h cortical.h interface.h
GDEPS = $(patsubst %,$(IDIR)/%,$(_GDEPS))

_OBJ = gyre.o cortical.o interface.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_SO = cortical.so interface.so 
SO = $(patsubst %,$(LDIR)/%,$(_SO))

$(ODIR)/%.o: %.c $(GDEPS)
	$(MPICC) -c -o $@ $< $(CFLAGS)
	
$(LDIR)/%.so: $(SODEPS)
	gcc -shared -o ../lib/cortical.so cortical.c -I../include
	gcc -shared -o ../lib/interface.so interface.c -I../include

gyre: $(OBJ)
	$(MPICC) -o $@ $^ $(CFLAGS) $(LIBS)

shared: $(SO)
	echo "shared"

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(LDIR)/*.so
