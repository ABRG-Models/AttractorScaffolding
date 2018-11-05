# Compiling parallelswing algorithm in facto, but in fact, I don't use
# it; I only use the wrapper in lmp.h
add_library(facto STATIC parallelswing.cpp primeswing.cpp)
target_link_libraries(facto mpir mpirxx)

add_executable(testfac testfac.cpp)
target_link_libraries(testfac facto)

add_executable(facs facs.cpp)
target_link_libraries(facs facto)

add_executable(binomial binomial.cpp)
target_link_libraries(binomial facto)

add_executable(factorial factorial.cpp)
target_link_libraries(factorial facto)

add_executable(fac_a_over_b fac_a_over_b.cpp)
target_link_libraries(fac_a_over_b facto)
