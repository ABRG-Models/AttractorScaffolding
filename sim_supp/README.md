# Simulation code - supplementary analysis

This directory contains addition simulation code which was used to
make additional analysis of the system reported in the paper.

## Supplementary programs

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

The program randomly selects a fixed number of genomes and finds the
fitness of each. The idea is to determine, by sampling, the
distribution of fitnesses for the chosen fitness function.

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

### h_m.cpp

This compiles a function h(m) which is relevant to fitness function 0.

* Compiles to **h_m**
* Results on command line

### mutatestream.cpp

Mutates a known-good genome 100 times with pOn=0.05 and outputs the
genomes in 1s and 0s format on the cmd line.

* Compiles to **mutatestream**
* Results on command line

### mutation.cpp

Mutate a known-good genome and evaluate the fitness at various Hamming
distances away from the f=1 original.

* Compiles into **mutation4**, **mutation5** and **mutation6**
* Results into data/mutations_ff4_n4_*.csv
* Results plotted by plot_smooth_fit.py

### prob_fitinc_bybits.cpp

Take N_Starts randomly generated, but f>0 genomes. Mutate each one
N_Generations times. Determine the probability distribution of
mutating to a fitter genome. The number of bits to flip each time is
provided on the command line.

* Compiles into **prob_fitinc_bybits**
* Results into data/prob_fitinc_*
* Results plotted by plot_prob_fitinc_flipbits.py

### prob_fitinc.cpp

Take N_Starts randomly generated, but f>0 genomes. Mutate each one
N_Generations times. Determine the probability distribution of
mutating to a fitter genome. The probability of a bit flip (pOn) is
provided on the command line.

* Compiles into **prob_fitinc**
* Results into data/prob_fitinc_*
* Results plotted by plot_prob_fitinc.py

### showfitness.cpp

Taking a genome in "1s and 0s format", convert to array<genosect_t,
N_Genes> and then evaluate the fitness. If no genome is provided on
the command line, generate a random one.

* Compiles into **showfitness**
* Results on command line.
