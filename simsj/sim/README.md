# Simulation code

This directory contains most of the important simulation code. The key
files to look at, when referring to the paper are the header files,
and the programs evolve.cpp, x.cpp, y.cpp. **FIXME**.

## Headers

The code is stored in the following headers:

### lib.h

Contains most of the functionality of the system, several global
variables, functions that set up masks, define the types used in the
program. Important functions such as random_genome(), zero_genome,
copy_genome() and evolve_genome() are found here.

### fitness.h and fitness4.h

This header includes the relevant fitness function based on #defines
set at compile time. The paper is based on fitness4.h, which contains
the function evaluate_fitness().

### basins.h

This header contains code to determine the transitions in all of the
basins of attraction found for a given genome. Contains the class
BasinOfAttraction and a function find_basins_of_attraction() which
finds all the basins for a given genome.

### endpoint.h

Contains a little code that is included both by fitness.h and by
basins.h.

### mutation.h

Contains functions to determine the number of fit mutated genomes that
exist * a Hamming distance m (or h) away from a given, fit
genome. Also contains the function evolve_new_genome, which, starting
from a random_genome, calls evolve_genome() until f=1.

## Simulation programs

Each of the .cpp files are compiled into a separate program (or
sometimes, into several related programs, with different #defines for
each).

The programs are:

### analyse_genome.cpp

Takes a genome, evolves it once with evolve_genome(), and shows an
analysis of the changes in transitions.

### bitflipping.cpp

Randomly generate a genome. Look at the number of changed transitions
for flipping each single bit. Also show the fitness of the bit-flipped
genome.
