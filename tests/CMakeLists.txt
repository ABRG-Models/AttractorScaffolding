add_executable(statechange statechange.cpp)
target_compile_definitions(statechange PUBLIC USE_FITNESS_1)
add_test(statechange statechange)

add_executable(hamming hamming.cpp)
add_test(hamming hamming)

add_executable(hamming_genome hamming_genome.cpp)
add_test(hamming_genome hamming_genome)

add_executable(tgenome2str genome2str.cpp)
add_test(tgenome2str tgenome2str)

add_executable(tstr2genome str2genome.cpp)
add_test(tstr2genome tstr2genome)

# Test FF 4 with 1000 genomes from file
add_executable(test_thousand test_thousand.cpp)
target_compile_definitions(test_thousand PUBLIC USE_FITNESS_4)
add_test(test_thousand test_thousand)

add_executable(state2string2state state2string2state.cpp)
add_test(state2string2state state2string2state)

add_executable(quine quine.cpp)
add_test(quine quine)
