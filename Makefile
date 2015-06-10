COMPILER = gcc
CCFLAGS = -ansi -pedantic -Wall -std=c99 -g

all: cache-sim

cache-sim: cache-sim.o 
	$(COMPILER) $(CCFLAGS) -o cache-sim cache-sim.o

cache-sim.o: cache-sim.c 
	$(COMPILER) $(CCFLAGS) -c cache-sim.c

clean:
	rm -f cache-sim
	rm -f *.o
