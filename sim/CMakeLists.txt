# The main evolve program, compiled with various fitness functions,
# but always with N_Genes=5. Operates in the "drift" case, where the
# evolution of a new genome of equal fitness to the old one DOES
# replace the old one.

# The main evolve program compiled to use json parameter config file
if (EXISTS ${JSONLIBLINK})

  # Have libjson.a/dylib; assume we have json/json.h in include path somewhere...
  message(INFO "We have JSON library to link against; compiling evolve_json and friends.")

  add_executable(evolve evolve.cpp)
  target_compile_definitions(evolve PUBLIC USE_FITNESS_4 N_Genes=5)
  target_link_libraries(evolve ${JSONLIBLINK})

  add_executable(evolve_withf evolve.cpp)
  target_compile_definitions(evolve_withf PUBLIC USE_FITNESS_4 N_Genes=5 RECORD_ALL_FITNESS)
  target_link_libraries(evolve_withf ${JSONLIBLINK})

  add_executable(evolve6 evolve.cpp)
  target_compile_definitions(evolve6 PUBLIC USE_FITNESS_4 N_Genes=6)
  target_link_libraries(evolve6 ${JSONLIBLINK})

  add_executable(evolve6_withf evolve.cpp)
  target_compile_definitions(evolve6_withf PUBLIC USE_FITNESS_4 N_Genes=6 RECORD_ALL_FITNESS)
  target_link_libraries(evolve6_withf ${JSONLIBLINK})
endif()

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
# Squash a warning which I've verified is not an error:
target_compile_options(proprandom3 PRIVATE -Wno-shift-count-overflow)
target_link_libraries(proprandom3 facto)
add_executable(proprandom4 proprandomfits.cpp)
target_compile_definitions(proprandom4 PUBLIC N_Genes=4 USE_FITNESS_4)
target_compile_options(proprandom4 PRIVATE -Wno-shift-count-overflow)
target_link_libraries(proprandom4 facto)
add_executable(proprandom5 proprandomfits.cpp)
target_compile_definitions(proprandom5 PUBLIC N_Genes=5 USE_FITNESS_4)
target_compile_options(proprandom5 PRIVATE -Wno-shift-count-overflow)
target_link_libraries(proprandom5 facto)
add_executable(proprandom6 proprandomfits.cpp)
target_compile_definitions(proprandom6 PUBLIC N_Genes=6 USE_FITNESS_4)
target_compile_options(proprandom6 PRIVATE -Wno-shift-count-overflow)
target_link_libraries(proprandom6 facto)

# Show the fitness of the selected genome for the paper
add_executable(showselected showselected.cpp)
target_compile_definitions(showselected PUBLIC USE_FITNESS_4)

# Analyse complexity of random genomes
add_executable(complexity_random complexity_random.cpp)
target_compile_definitions(complexity_random PUBLIC N_Genes=5)

# Analyse complexity of fit genomes
add_executable(complexity_fit complexity_fit.cpp)
target_compile_definitions(complexity_fit PUBLIC N_Genes=5 USE_FITNESS_4)
