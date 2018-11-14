# The main evolve program, compiled with various fitness functions,
# but always with N_Genes=5
add_executable(evolve_ff1 evolve.cpp) # Uses 'fitness function 1' by default
target_compile_definitions(evolve_ff1 PUBLIC STIPULATE_DRIFT_CASE USE_FITNESS_1)
add_executable(evolve_ff0 evolve.cpp)
target_compile_definitions(evolve_ff0 PUBLIC STIPULATE_DRIFT_CASE USE_FITNESS_0)
add_executable(evolve_ff2 evolve.cpp)
target_compile_definitions(evolve_ff2 PUBLIC STIPULATE_DRIFT_CASE USE_FITNESS_2)
add_executable(evolve_ff3 evolve.cpp)
target_compile_definitions(evolve_ff3 PUBLIC STIPULATE_DRIFT_CASE USE_FITNESS_3)
add_executable(evolve_ff4 evolve.cpp)
target_compile_definitions(evolve_ff4 PUBLIC STIPULATE_DRIFT_CASE USE_FITNESS_4)

add_executable(evolve_nodrift_ff4 evolve.cpp)
target_compile_definitions(evolve_nodrift_ff4 PUBLIC USE_FITNESS_4)

# Like the evolve programs, but saving every fitness value at every
# generation. Again, N_Genes=5.
add_executable(evolve_withf_ff0 evolve.cpp)
target_compile_definitions(evolve_withf_ff0 PUBLIC USE_FITNESS_0 RECORD_ALL_FITNESS N_Generations=100000)
add_executable(evolve_withf_ff4 evolve.cpp)
target_compile_definitions(evolve_withf_ff4 PUBLIC USE_FITNESS_4 RECORD_ALL_FITNESS N_Generations=100000)

# To generate the data for allowing the genome to drift.
add_executable(drift_ff0 drift.cpp)
target_compile_definitions(drift_ff0 PUBLIC USE_FITNESS_0)
add_executable(drift_ff1 drift.cpp)
target_compile_definitions(drift_ff1 PUBLIC USE_FITNESS_1)
add_executable(drift_ff4 drift.cpp)
target_compile_definitions(drift_ff4 PUBLIC USE_FITNESS_4)

add_executable(drift_withf_ff4 drift.cpp)
target_compile_definitions(drift_withf_ff4 PUBLIC USE_FITNESS_4 RECORD_ALL_FITNESS N_Generations=100000)

# Null model - repeatedly generate random genomes
add_executable(null_withf_ff4 nullmodel.cpp)
target_compile_definitions(null_withf_ff4 PUBLIC USE_FITNESS_4 RECORD_ALL_FITNESS N_Generations=100000)


# This is essentially a debugging program, which sets up a random
# genome, and evolves it one generation only.
add_executable(evolve_onegen_ff0 evolve_onegen.cpp)
target_compile_definitions(evolve_onegen_ff0 PUBLIC USE_FITNESS_0)
add_executable(evolve_onegen_ff1 evolve_onegen.cpp)
target_compile_definitions(evolve_onegen_ff1 PUBLIC USE_FITNESS_1)
add_executable(evolve_onegen_ff2 evolve_onegen.cpp)
target_compile_definitions(evolve_onegen_ff2 PUBLIC USE_FITNESS_2)
add_executable(evolve_onegen_ff3 evolve_onegen.cpp)
target_compile_definitions(evolve_onegen_ff3 PUBLIC USE_FITNESS_3)
add_executable(evolve_onegen_ff4 evolve_onegen.cpp)
target_compile_definitions(evolve_onegen_ff4 PUBLIC USE_FITNESS_4)

# This starts with a random genome, and evolves it for as many
# generations as it takes to get to the first fit genome. N_Genes=5.
add_executable(evolve_fit_genome_ff0 evolve_fit_genome.cpp)
target_compile_definitions(evolve_fit_genome_ff0 PUBLIC USE_FITNESS_0)
add_executable(evolve_fit_genome_ff1 evolve_fit_genome.cpp)
target_compile_definitions(evolve_fit_genome_ff1 PUBLIC USE_FITNESS_1)
add_executable(evolve_fit_genome_ff2 evolve_fit_genome.cpp)
target_compile_definitions(evolve_fit_genome_ff2 PUBLIC USE_FITNESS_2)
add_executable(evolve_fit_genome_ff4 evolve_fit_genome.cpp)
target_compile_definitions(evolve_fit_genome_ff4 PUBLIC USE_FITNESS_4)

# This evaluates the fitness of the specific genome displayed in
# Fig. 1 of the paper. N_Genes=5.
add_executable(onestate_ff1 onestate.cpp)
target_compile_definitions(onestate_ff1 PUBLIC USE_FITNESS_1)
add_executable(onestate_ff0 onestate.cpp)
target_compile_definitions(onestate_ff0 PUBLIC USE_FITNESS_0)
add_executable(onestate_ff4 onestate.cpp)
target_compile_definitions(onestate_ff4 PUBLIC USE_FITNESS_4)

# For N_Genes=5,Randomly select a number of genomes and output numbers
# for those with fitness 0, those with >0 fitness and those with
# fitness == 1.
add_executable(estimate_zero_fits_ff0 estimate_zero_fits.cpp)
target_compile_definitions(estimate_zero_fits_ff0 PUBLIC USE_FITNESS_0)
add_executable(estimate_zero_fits_ff1 estimate_zero_fits.cpp)
target_compile_definitions(estimate_zero_fits_ff1 PUBLIC USE_FITNESS_1)
add_executable(estimate_zero_fits_ff4 estimate_zero_fits.cpp)
target_compile_definitions(estimate_zero_fits_ff4 PUBLIC USE_FITNESS_4)

# Show the fitness of a genome passed in on the command line.
add_executable(showfitness_ff0 showfitness.cpp)
target_compile_definitions(showfitness_ff0 PUBLIC USE_FITNESS_0)
add_executable(showfitness_ff1 showfitness.cpp)
target_compile_definitions(showfitness_ff1 PUBLIC USE_FITNESS_1)
add_executable(showfitness_ff4 showfitness.cpp)
target_compile_definitions(showfitness_ff4 PUBLIC USE_FITNESS_4)

#
# Evaluate the fitness of mutations away from a F=1 genome for Hamming
# distance 1,2,3, etc. Do so for N_Genes=4,5,6.
#

# Fitness function 0
# For N_Genes=4
add_executable(mutation4_ff0 mutation.cpp)
target_compile_definitions(mutation4_ff0 PUBLIC N_Genes=4 USE_FITNESS_0)
target_link_libraries(mutation4_ff0 facto)
# For N_Genes=5
add_executable(mutation5_ff0 mutation.cpp)
target_compile_definitions(mutation5_ff0 PUBLIC N_Genes=5 USE_FITNESS_0)
target_link_libraries(mutation5_ff0 facto)
# For N_Genes=6
add_executable(mutation6_ff0 mutation.cpp)
target_compile_definitions(mutation6_ff0 PUBLIC N_Genes=6 USE_FITNESS_0)
target_link_libraries(mutation6_ff0 facto)

# Fitness function 1
# For N_Genes=4
add_executable(mutation4_ff1 mutation.cpp)
target_compile_definitions(mutation4_ff1 PUBLIC N_Genes=4 USE_FITNESS_1)
target_link_libraries(mutation4_ff1 facto)
# For N_Genes=5
add_executable(mutation5_ff1 mutation.cpp)
target_compile_definitions(mutation5_ff1 PUBLIC N_Genes=5 USE_FITNESS_1)
target_link_libraries(mutation5_ff1 facto)
# For N_Genes=6
add_executable(mutation6_ff1 mutation.cpp)
target_compile_definitions(mutation6_ff1 PUBLIC N_Genes=6 USE_FITNESS_1)
target_link_libraries(mutation6_ff1 facto)

# Fitness function 4
# For N_Genes=4
add_executable(mutation4_ff4 mutation.cpp)
target_compile_definitions(mutation4_ff4 PUBLIC N_Genes=4 USE_FITNESS_4)
target_link_libraries(mutation4_ff4 facto)
# For N_Genes=5
add_executable(mutation5_ff4 mutation.cpp)
target_compile_definitions(mutation5_ff4 PUBLIC N_Genes=5 USE_FITNESS_4)
target_link_libraries(mutation5_ff4 facto)
# For N_Genes=6
add_executable(mutation6_ff4 mutation.cpp)
target_compile_definitions(mutation6_ff4 PUBLIC N_Genes=6 USE_FITNESS_4)
target_link_libraries(mutation6_ff4 facto)

# Find the proportion of random genomes with F=1, F>0; fitness function 0
add_executable(proprandom4_ff0 proprandomfits.cpp)
target_compile_definitions(proprandom4_ff0 PUBLIC N_Genes=4 USE_FITNESS_0)
add_executable(proprandom5_ff0 proprandomfits.cpp)
target_compile_definitions(proprandom5_ff0 PUBLIC N_Genes=5 USE_FITNESS_0)
add_executable(proprandom6_ff0 proprandomfits.cpp)
target_compile_definitions(proprandom6_ff0 PUBLIC N_Genes=6 USE_FITNESS_0)

# Find the proportion of random genomes with F=1, F>0; fitness function 1
add_executable(proprandom4_ff1 proprandomfits.cpp)
target_compile_definitions(proprandom4_ff1 PUBLIC N_Genes=4 USE_FITNESS_1)
add_executable(proprandom5_ff1 proprandomfits.cpp)
target_compile_definitions(proprandom5_ff1 PUBLIC N_Genes=5 USE_FITNESS_1)
add_executable(proprandom6_ff1 proprandomfits.cpp)
target_compile_definitions(proprandom6_ff1 PUBLIC N_Genes=6 USE_FITNESS_1)

# Find the proportion of random genomes with F=1, F>0; fitness function 4
add_executable(proprandom4_ff4 proprandomfits.cpp)
target_compile_definitions(proprandom4_ff4 PUBLIC N_Genes=4 USE_FITNESS_4)
add_executable(proprandom5_ff4 proprandomfits.cpp)
target_compile_definitions(proprandom5_ff4 PUBLIC N_Genes=5 USE_FITNESS_4)
add_executable(proprandom6_ff4 proprandomfits.cpp)
target_compile_definitions(proprandom6_ff4 PUBLIC N_Genes=6 USE_FITNESS_4)

# This executable displays the genome as it is mutated in a stream of text.
add_executable(mutatestream mutatestream.cpp)
target_compile_definitions(mutatestream PUBLIC USE_FITNESS_1)

# This accepts a string version of the genome (in 1s and 0s) and
# displays the genome in tabular form.
add_executable(str2genome str2genome.cpp)

# This computes a function h(m) which may be discussed in the paper.
add_executable(h_m h_m.cpp)
target_link_libraries(h_m facto)

# Combinations
add_executable(combos combos.cpp)
target_link_libraries(combos facto)

# Computing Probability of non-zero score by fast, binomials method:
add_executable(compute_pnot0 compute_pnot0.cpp)
target_link_libraries(compute_pnot0 facto)

# Analysing the genome and the basins of attraction that they create.
add_executable(analyse_genome analyse_genome.cpp)

add_executable(prob_fitinc prob_fitinc.cpp)
target_compile_definitions(prob_fitinc PUBLIC N_Genes=5 USE_FITNESS_4)

add_executable(fitness_of_ten fitness_of_ten.cpp)
target_compile_definitions(fitness_of_ten PUBLIC N_Genes=5 USE_FITNESS_4)

add_executable(flip_stats flip_stats.cpp)
target_compile_definitions(flip_stats PUBLIC N_Genes=5 N_Generations=100000 USE_FITNESS_4)
