# The main evolve program, compiled with various fitness functions,
# but always with N_Genes=5. Operates in the "drift" case, where the
# evolution of a new genome of equal fitness to the old one DOES
# replace the old one.
add_executable(evolve evolve.cpp)
target_compile_definitions(evolve PUBLIC STIPULATE_DRIFT_CASE USE_FITNESS_4)

# The main evolve program operating in the "nodrift" case, where the
# evolution of a new genome of equal fitness to the old one does NOT
# replace the old one.
add_executable(evolve_nodrift evolve.cpp)
target_compile_definitions(evolve_nodrift PUBLIC USE_FITNESS_4)

add_executable(evolve_nodrift_n6 evolve.cpp)
target_compile_definitions(evolve_nodrift_n6 PUBLIC N_Genes=6 USE_FITNESS_4)

add_executable(evolve_nodrift_n7 evolve.cpp)
target_compile_definitions(evolve_nodrift_n7 PUBLIC N_Genes=7 USE_FITNESS_4)

# Like the evolve programs, but saving every fitness value at every
# generation. When fitness reaches 1, start from a new random genome
# and create a new file to save the data into. Again, N_Genes=5.
add_executable(evolve_withf evolve.cpp)
target_compile_definitions(evolve_withf PUBLIC STIPULATE_DRIFT_CASE USE_FITNESS_4 RECORD_ALL_FITNESS N_Generations=1000000)
# And nodrift version of the above:
add_executable(evolve_nodrift_withf evolve.cpp)
target_compile_definitions(evolve_nodrift_withf PUBLIC USE_FITNESS_4 RECORD_ALL_FITNESS N_Generations=1000000)

# To generate the data for allowing the genome to drift.
add_executable(drift drift.cpp)
target_compile_definitions(drift PUBLIC USE_FITNESS_4)

add_executable(drift_withf drift.cpp)
target_compile_definitions(drift_withf PUBLIC USE_FITNESS_4 RECORD_ALL_FITNESS N_Generations=100000)

# Null model - repeatedly generate random genomes
add_executable(null_withf nullmodel.cpp)
target_compile_definitions(null_withf PUBLIC USE_FITNESS_4 RECORD_ALL_FITNESS N_Generations=100000)

# This is essentially a debugging/example program, which sets up a
# random genome, and evolves it one generation only.
add_executable(evolve_onegen evolve_onegen.cpp)
target_compile_definitions(evolve_onegen PUBLIC USE_FITNESS_4)

# This starts with a random genome, and evolves it for as many
# generations as it takes to get to the first fit genome. N_Genes=5.
add_executable(evolve_fit_genome evolve_fit_genome.cpp)
target_compile_definitions(evolve_fit_genome PUBLIC USE_FITNESS_4)

# This evaluates the fitness of the specific genome displayed in
# Fig. 1 of the paper. N_Genes=5.
add_executable(onestate onestate.cpp)
target_compile_definitions(onestate PUBLIC USE_FITNESS_4)

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

# Find the proportion of random genomes with F=1, F>0; fitness function 4
add_executable(proprandom4 proprandomfits.cpp)
target_compile_definitions(proprandom4 PUBLIC N_Genes=4 USE_FITNESS_4)
add_executable(proprandom5 proprandomfits.cpp)
target_compile_definitions(proprandom5 PUBLIC N_Genes=5 USE_FITNESS_4)
add_executable(proprandom6 proprandomfits.cpp)
target_compile_definitions(proprandom6 PUBLIC N_Genes=6 USE_FITNESS_4)

# This executable displays the genome as it is mutated in a stream of text.
add_executable(mutatestream mutatestream.cpp)
target_compile_definitions(mutatestream PUBLIC USE_FITNESS_4)

# This accepts a string version of the genome (in 1s and 0s) and
# displays the genome in tabular form.
add_executable(str2genome str2genome.cpp)

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

# Analysing the genome and the basins of attraction that they create.
add_executable(analyse_genome analyse_genome.cpp)

# Compute the probability of the fitness increasing on the next
# evolution, starting from a fixed number of random genomes of
# non-zero fitness.
add_executable(prob_fitinc prob_fitinc.cpp)
target_compile_definitions(prob_fitinc PUBLIC N_Genes=5 USE_FITNESS_4)

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
