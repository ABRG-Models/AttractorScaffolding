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
the function evaluate_fitness(). Other fitness functions have been
investigated and these are also present (fitness[1-8].h)

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

* Compiles to: **analyse_genome**
* Results on command line

### bitflipping.cpp

Randomly generate a genome. Look at the number of changed transitions
for flipping each single bit. Also show the fitness of the bit-flipped
genome.

* Compiles to: **bitflipping**
* Results on command line

### combos.cpp

Combinatorics. This was used when trying to determine the
probability of getting zero fitness with fitness function 4 for a
given number of genes. There's an odd bug in this program and so it
fails to run at present.

* Compiles to: **combos**
* Results on command line

### compute_pnot0.cpp

Computes the probability of non-zero fitness in the fitness function
number 4 as a function of the limit cycle length of the attractor into
which the initial state transitions. This computes the value of the
relation given in paper/combinatorics/combinatorics.tex/pdf for a
given number of genes supplied on the command line. Saves results in,
e.g. pnot0_n5.csv for a call like `compute_pnot0 5`

* Compiles to: **compute_pnot0**
* Results plotted by **plot_p0.py**

### dimension_tree.cpp

Compute, by brute force, the tree of nodes and edges for an n
dimensional boolean space. Start at the node [000...0000] and
proceed with incrementing Hamming distance from the start. Make
sets containing the layer nodes and the transitions.

Saves out data in a form suitable for graphing in python in
data/node_trans_layer*.csv

* Compiles to: **dimension_tree**
* Results plotted by **plot_dimension_tree.py**

### drift.cpp

The drifting algorithm evolves the genome at every generation,
recording how often an f=1 genome is happened upon. With the drift
algorithm, all p values should give the same mean time to f=1.

* Compiles to: **drift**
* Results plotted by **plot_drift.py**

### estimate_fitness_dist.cpp

Estimate the distribution of fitness values for a given fitness
function. The fitness function to estimate is set at compile time, so
several versions of this program are compiled.

* Compiles to: **estimate_fitness_dist_ff4**,  **estimate_fitness_dist_ff5**, etc
* Results plotted by **plot_fitness_dists.py**

### estimate_shared_terminations.cpp

Estimate the proportion of genomes for which both anterior and
posterior initial states will end on the same limit cycle - those
genomes which have 'shared terminations'. Defaults to n=5 system, n=6
and n=7 versions are compiled.

* Compiles to: **estimate_shared_terminations**,  **estimate_shared_terminations_n6**,  **estimate_shared_terminations_n7**
* Results on command line.

### estimate_zero_fits.cpp

Randomly selects 1000000 genomes and finds numbers for those with
fitness 0, those with >0 fitness and those with fitness == 1.

* Compiles to: **estimate_zero_fits**
* Results on command line.

### evolve_bybits.cpp

This is like evolve.cpp, except that instead of flipping bits with a
fixed probability, it flips a fixed number of randomly selected bits
in the genome on each evolution.

* Compiles to **evolve_bybits** and **evolve_bybits_nodrift**
* Results into: data/evolveb_a21_p10_ff4*.csv or data/evolveb_nodrift_a21_p10_ff4*.csv
* Results plotted with: plot_evospeed_bf.py or plot_evospeed_histo_only_multi_bf.py

### evolve.cpp

The main algorithm reported in the paper.

Evolves genomes by flipping bits with a probability supplied at the
command line and accepting changes if the evolved genome specifies a
network whose fitness has increased (nodrift) or has stayed the same
or increased (drift).

* Compiles to **evolve** and **evolve_nodrift**
* Results into: data/evolve_a21_p10_ff4*.csv or data/evolve_nodrift_a21_p10_ff4*.csv
* Results plotted with plot_evospeed.py, plot_evospeed_histo_only_multi.py and plot_evospeed_powerlaw.py

### evolve_fit_genome.cpp

Starting from a random genome, evolve it into an f=1 genome once
only. Show the resulting genome on stdout.

* Compiles to **evolve_fit_genome**
* Results on command line.

### evolve_onegen.cpp

Run a single evolution step, showing the genome before and after.

* Compiles to **evolve_onegen**
* Results on command line.

### fitness_of_ten.cpp

Generates 10 non-zero fit genomes and show them, along with their
fitness.

* Compiles to **fitness_of_ten**
* Results on command line.

### flip_stats.cpp

Tests the bit flipping statistics, to ensure that bit flips are evenly
distributed amount the N_Genes genosects.

* Compiles to **flip_stats**
* Results on command line.

### genome2str.cpp

Convert from genome_id format
(e.g. 5039a8e4-a090a0eb-56cfd0a8-9c9ccdbb-60b214b) to 1s and 0s
format.

* Compiles to **genome2str**
* Results on command line

### h_m.cpp

This compiles a function h(m) which is relevant to fitness function 0.

* Compiles to **h_m**
* Results on command line
