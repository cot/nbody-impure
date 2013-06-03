CC     = gcc
CFLAGS = -lm -W -Wall -ansi -pedantic -std=c99 -m64 
LFLAGS = -fPIC -shared

SRC    = src
BIN    = bin
INC    = -I/inc

all: tout

papi: CC+= -DPAPI -lpapi
papi: tout

check: CC+= -DCHECK
check: tout

profile: CC += -DPROFILE -g -pg
profile: tout

tout: CC += -msse4 -O2 -ftree-vectorize -ftree-vectorizer-verbose=5 -funroll-loops
tout: nbody

optim: CC += -O3
optim: nbody

nbody: $(SRC)/nbody.c 
	$(CC) $(INC) $(SRC)/nbody.c -o $(BIN)/nbody $(CFLAGS)

clean:
	rm -rf *.o $(BIN)/nbody 

