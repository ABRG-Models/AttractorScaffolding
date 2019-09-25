# Evolving by flipping fixed number of bits each time (with drift)...
add_executable(evolve_bybits evolve_bybits.cpp)
target_compile_definitions(evolve_bybits PUBLIC STIPULATE_DRIFT_CASE USE_FITNESS_4)

# ...(without drift)
add_executable(evolve_bybits_nodrift evolve_bybits.cpp)
target_compile_definitions(evolve_bybits_nodrift PUBLIC USE_FITNESS_4)

# To generate the data for allowing the genome to drift.
add_executable(drift drift.cpp)
target_compile_definitions(drift PUBLIC USE_FITNESS_4)

# Record drift with all fitness saved
add_executable(drift_withf drift.cpp)
target_compile_definitions(drift_withf PUBLIC USE_FITNESS_4 RECORD_ALL_FITNESS N_Generations=100000)

# For N_Genes=5,Randomly select a number of genomes and output numbers
# for those with fitness 0, those with >0 fitness and those with
# fitness == 1.
add_executable(estimate_zero_fits estimate_zero_fits.cpp)
target_compile_definitions(estimate_zero_fits PUBLIC USE_FITNESS_4)

# Estimate how many genomes lead to "shared terminations"
add_executable(estimate_shared_terminations estimate_shared_terminations.cpp)
target_compile_definitions(estimate_shared_terminations PUBLIC USE_FITNESS_4)

add_executable(estimate_shared_terminations_n6 estimate_shared_terminations.cpp)
target_compile_definitions(estimate_shared_terminations_n6 PUBLIC USE_FITNESS_4 N_Genes=6)

add_executable(estimate_shared_terminations_n7 estimate_shared_terminations.cpp)
target_compile_definitions(estimate_shared_terminations_n7 PUBLIC USE_FITNESS_4 N_Genes=7)

# Show the fitness of a genome passed in on the command line.
add_executable(showfitness showfitness.cpp)
target_compile_definitions(showfitness PUBLIC USE_FITNESS_4)

#
# Evaluate the fitness of mutations away from a F=1 genome for Hamming
# distance 1,2,3, etc. Do so for N_Genes=4,5,6.
#

# Fitness function 4
# For N_Genes=4
add_executable(mutation4 mutation.cpp)
target_compile_definitions(mutation4 PUBLIC N_Genes=4 USE_FITNESS_4)
target_link_libraries(mutation4 facto)
# For N_Genes=5
add_executable(mutation5 mutation.cpp)
target_compile_definitions(mutation5 PUBLIC N_Genes=5 USE_FITNESS_4)
target_link_libraries(mutation5 facto)
# For N_Genes=6
add_executable(mutation6 mutation.cpp)
target_compile_definitions(mutation6 PUBLIC N_Genes=6 USE_FITNESS_4)
target_link_libraries(mutation6 facto)

# The null model.
add_executable(nullmodel nullmodel.cpp)
target_compile_definitions(nullmodel PUBLIC N_Genes=5 USE_FITNESS_4)

add_executable(nullmodel_withf nullmodel.cpp)
target_compile_definitions(nullmodel_withf PUBLIC USE_FITNESS_4 RECORD_ALL_FITNESS N_Generations=100000)

# This executable displays the genome as it is mutated in a stream of text.
add_executable(mutatestream mutatestream.cpp)
target_compile_definitions(mutatestream PUBLIC USE_FITNESS_4)

# This computes a function h(m) which may be discussed in the paper.
add_executable(h_m h_m.cpp)
target_link_libraries(h_m facto)

# Combinations
add_executable(combos combos.cpp)
target_link_libraries(combos facto)

# Building dimension trees to work out all the ways to traverse the
# genome space to some given point.
add_executable(dimension_tree dimension_tree.cpp)
target_link_libraries(dimension_tree)

# Computing Probability of non-zero score for fitness function 4 by
# fast, binomials method:
add_executable(compute_pnot0 compute_pnot0.cpp)
target_link_libraries(compute_pnot0 facto)

add_executable(bitflipping bitflipping.cpp)
target_link_libraries(bitflipping)
target_compile_definitions(bitflipping PUBLIC N_Genes=5 USE_FITNESS_4)

# Analysing the genome and the basins of attraction that they create.
add_executable(analyse_genome analyse_genome.cpp)

# Compute the probability of the fitness increasing on the next
# evolution, starting from a fixed number of random genomes of
# non-zero fitness.
add_executable(prob_fitinc prob_fitinc.cpp)
target_compile_definitions(prob_fitinc PUBLIC N_Genes=5 USE_FITNESS_4)

add_executable(prob_fitinc_bybits prob_fitinc_bybits.cpp)
target_compile_definitions(prob_fitinc_bybits PUBLIC N_Genes=5 USE_FITNESS_4)

# Generates 10, random, non-zero fitness genomes and displays them.
add_executable(fitness_of_ten fitness_of_ten.cpp)
target_compile_definitions(fitness_of_ten PUBLIC N_Genes=5 USE_FITNESS_4)

# Examine the statistics of the evolution code - make sure that bits
# are flipped with even probability.
add_executable(flip_stats flip_stats.cpp)
target_compile_definitions(flip_stats PUBLIC N_Genes=5 N_Generations=100000 USE_FITNESS_4)

#
# Estimate fitness function score distributions by sampling
#
add_executable(estimate_fitness_dist_ff4 estimate_fitness_dist.cpp)
target_compile_definitions(estimate_fitness_dist_ff4 PUBLIC N_Genes=5 N_Genomes=10000000 USE_FITNESS_4)

add_executable(estimate_fitness_dist_ff5 estimate_fitness_dist.cpp)
target_compile_definitions(estimate_fitness_dist_ff5 PUBLIC N_Genes=5 N_Genomes=10000000 USE_FITNESS_5)

add_executable(estimate_fitness_dist_ff6 estimate_fitness_dist.cpp)
target_compile_definitions(estimate_fitness_dist_ff6 PUBLIC N_Genes=5 N_Genomes=10000000 USE_FITNESS_6)

add_executable(estimate_fitness_dist_ff7 estimate_fitness_dist.cpp)
target_compile_definitions(estimate_fitness_dist_ff7 PUBLIC N_Genes=5 N_Genomes=10000000 USE_FITNESS_7)

add_executable(estimate_fitness_dist_ff8 estimate_fitness_dist.cpp)
target_compile_definitions(estimate_fitness_dist_ff8 PUBLIC N_Genes=5 N_Genomes=10000000 USE_FITNESS_8)

# This accepts a string version of the genome (in 1s and 0s) and
# displays the genome in tabular form.
add_executable(str2genome str2genome.cpp)
add_executable(str2genome6 str2genome.cpp)
target_compile_definitions(str2genome6 PUBLIC N_Genes=6)

# The opposite of str2genome.
add_executable(genome2str genome2str.cpp)
add_executable(genome2str6 genome2str.cpp)
target_compile_definitions(genome2str6 PUBLIC N_Genes=6)
