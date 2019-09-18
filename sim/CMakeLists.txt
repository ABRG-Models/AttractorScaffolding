# The main evolve program, compiled with various fitness functions,
# but always with N_Genes=5. Operates in the "drift" case, where the
# evolution of a new genome of equal fitness to the old one DOES
# replace the old one.
add_executable(evolve evolve.cpp)
target_compile_definitions(evolve PUBLIC STIPULATE_DRIFT_CASE USE_FITNESS_4)

add_executable(evolve6 evolve.cpp)
target_compile_definitions(evolve6 PUBLIC STIPULATE_DRIFT_CASE USE_FITNESS_4 N_Genes=6)

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
target_compile_definitions(evolve_withf PUBLIC STIPULATE_DRIFT_CASE USE_FITNESS_4 RECORD_ALL_FITNESS N_Generations=10000)
# And nodrift version of the above:
add_executable(evolve_nodrift_withf evolve.cpp)
target_compile_definitions(evolve_nodrift_withf PUBLIC USE_FITNESS_4 RECORD_ALL_FITNESS N_Generations=100000)

# This is essentially a debugging/example program, which sets up a
# random genome, and evolves it one generation only.
add_executable(evolve_onegen evolve_onegen.cpp)
target_compile_definitions(evolve_onegen PUBLIC USE_FITNESS_4)

add_executable(evolve_onegen6 evolve_onegen.cpp)
target_compile_definitions(evolve_onegen6 PUBLIC USE_FITNESS_4 N_Genes=6)

# This starts with a random genome, and evolves it for as many
# generations as it takes to get to the first fit genome. N_Genes=5.
add_executable(evolve_fit_genome evolve_fit_genome.cpp)
target_compile_definitions(evolve_fit_genome PUBLIC USE_FITNESS_4)

add_executable(evolve_fit_genome6 evolve_fit_genome.cpp)
target_compile_definitions(evolve_fit_genome6 PUBLIC USE_FITNESS_4 N_Genes=6)

# Find the proportion of random genomes with F=1, F>0; fitness function 4
add_executable(proprandom3 proprandomfits.cpp)
target_compile_definitions(proprandom3 PUBLIC N_Genes=3 USE_FITNESS_4)
target_link_libraries(proprandom3 facto)
add_executable(proprandom4 proprandomfits.cpp)
target_compile_definitions(proprandom4 PUBLIC N_Genes=4 USE_FITNESS_4)
target_link_libraries(proprandom4 facto)
add_executable(proprandom5 proprandomfits.cpp)
target_compile_definitions(proprandom5 PUBLIC N_Genes=5 USE_FITNESS_4)
target_link_libraries(proprandom5 facto)
add_executable(proprandom6 proprandomfits.cpp)
target_compile_definitions(proprandom6 PUBLIC N_Genes=6 USE_FITNESS_4)
target_link_libraries(proprandom6 facto)

# Show the fitness of the selected genome for the paper
add_executable(showselected showselected.cpp)
target_compile_definitions(showselected PUBLIC USE_FITNESS_4)

# This accepts a string version of the genome (in 1s and 0s) and
# displays the genome in tabular form.
add_executable(str2genome str2genome.cpp)
add_executable(str2genome6 str2genome.cpp)
target_compile_definitions(str2genome6 PUBLIC N_Genes=6)

# The opposite of str2genome.
add_executable(genome2str genome2str.cpp)
add_executable(genome2str6 genome2str.cpp)
target_compile_definitions(genome2str6 PUBLIC N_Genes=6)
