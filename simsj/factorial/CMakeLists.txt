# Compiling parallelswing algorithm in facto, but in fact, I don't use
# it; I only use the wrapper in lmp.h
add_library(facto SHARED parallelswing.cpp primeswing.cpp)
target_link_libraries(facto mpir mpirxx)

add_executable(testfac testfac.cpp)
target_link_libraries(testfac facto)

add_executable(facs facs.cpp)
target_link_libraries(facs facto)
