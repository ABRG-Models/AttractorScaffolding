Scripts to generate supplementary results

### run_prob_fitinc.sh

Run the prob_fitinc program for several values of p. This takes
N_Starts randomly generated, but f>0 genomes. It mutates each one
N_Generations times. Determines the probability distribution of
mutating to a fitter genome.

### run_compute_pnot0.sh

Compute the probability of f>0 for fitness function 4 (the one
reported in the paper).

### rundrift.sh

Allow the genome to drift, with no attention paid to the fitness of
the resulting systems.

### runmutations4.sh, runmutations5.sh, runmutations6.sh

Mutate a known-good genome and evaluate the fitness at various Hamming
distances away from the fitness = 1 original. Scripts to run for n=4,
n=5 and n=6 genes.
