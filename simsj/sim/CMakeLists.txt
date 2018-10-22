add_executable(evolve evolve.cpp)
add_executable(evolve_ff0 evolve.cpp)
target_compile_definitions(evolve_ff0 PUBLIC USE_FITNESS_0)

add_executable(drift drift.cpp)
add_executable(drift_ff0 drift.cpp)
target_compile_definitions(drift_ff0 PUBLIC USE_FITNESS_0)

#add_executable(random random.cpp)
#add_executable(random_ff0 random.cpp)
#target_compile_definitions(random_ff0 PUBLIC USE_FITNESS_0)

add_executable(evolve_onegen evolve_onegen.cpp)
add_executable(evolve_onegen_ff0 evolve_onegen.cpp)
target_compile_definitions(evolve_onegen_ff0 PUBLIC USE_FITNESS_0)

add_executable(evolve_fit_genome evolve_fit_genome.cpp)
add_executable(evolve_fit_genome_ff0 evolve_fit_genome.cpp)
target_compile_definitions(evolve_fit_genome_ff0 PUBLIC USE_FITNESS_0)

add_executable(onestate onestate.cpp)
add_executable(onestate_ff0 onestate.cpp)
target_compile_definitions(onestate_ff0 PUBLIC USE_FITNESS_0)

# For N_Genes=5
add_executable(estimate_zero_fits estimate_zero_fits.cpp)
add_executable(estimate_zero_fits_ff0 estimate_zero_fits.cpp)
target_compile_definitions(estimate_zero_fits_ff0 PUBLIC USE_FITNESS_0)

add_executable(str2genome str2genome.cpp)

add_executable(showfitness showfitness.cpp)
add_executable(showfitness_ff0 showfitness.cpp)
target_compile_definitions(showfitness_ff0 PUBLIC USE_FITNESS_0)

add_executable(mutatestream mutatestream.cpp)

# For N_Genes=4
add_executable(mutation4 mutation.cpp)
target_compile_definitions(mutation4 PUBLIC N_Genes=4)
target_link_libraries(mutation4 facto)
# For N_Genes=5
add_executable(mutation5 mutation.cpp)
target_compile_definitions(mutation5 PUBLIC N_Genes=5)
target_link_libraries(mutation5 facto)
# For N_Genes=6
add_executable(mutation6 mutation.cpp)
target_compile_definitions(mutation6 PUBLIC N_Genes=6)
target_link_libraries(mutation6 facto)

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

add_executable(proprandom4 proprandomfits.cpp)
target_compile_definitions(proprandom4 PUBLIC N_Genes=4)
add_executable(proprandom5 proprandomfits.cpp)
target_compile_definitions(proprandom5 PUBLIC N_Genes=5)
add_executable(proprandom6 proprandomfits.cpp)
target_compile_definitions(proprandom6 PUBLIC N_Genes=6)


add_executable(proprandom_fit0_4 proprandomfits.cpp)
target_compile_definitions(proprandom_fit0_4 PUBLIC N_Genes=4 USE_FITNESS_0)
add_executable(proprandom_fit0_5 proprandomfits.cpp)
target_compile_definitions(proprandom_fit0_5 PUBLIC N_Genes=5 USE_FITNESS_0)
add_executable(proprandom_fit0_6 proprandomfits.cpp)
target_compile_definitions(proprandom_fit0_6 PUBLIC N_Genes=6 USE_FITNESS_0)
