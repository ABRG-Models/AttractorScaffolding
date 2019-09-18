# AUTOMOC requires 2.8.6. Per-target include directories would require 2.8.8
cmake_minimum_required(VERSION 2.8.8)
project(BooleanNets)

#message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
#message(STATUS "  (This can be changed with `cmake -DCMAKE_INSTALL_PREFIX=/some/place`")

# From CMAKE_SYSTEM work out which of __OSX__, __GLN__, __NIX__ or
# __WIN__ are required
message(STATUS "Operating system: " ${CMAKE_SYSTEM})
if(CMAKE_SYSTEM MATCHES Linux.*)
  set(MORPH_HOST_DEFINITION "-D__GLN__")
elseif(CMAKE_SYSTEM MATCHES BSD.*)
  set(MORPH_HOST_DEFINITION "-D__NIX__")
elseif(APPLE)
  set(MORPH_HOST_DEFINITION "-D__OSX__")
elseif(CMAKE_SYSTEM MATCHES Win.*)
  set(MORPH_HOST_DEFINITION "-D__WIN__")
else()
  message(ERROR "Operating system not supported: " ${CMAKE_SYSTEM})
endif()

# Add the host definition to CXXFLAGS along with other switches
if (APPLE)
  # I'm not sure how to enable openmp on clang, so add whatever is
  # required here (and check process.cpp to correctly include the
  # omp.h header)
  set(CMAKE_CXX_FLAGS "${MORPH_HOST_DEFINITION} -Wall -g -std=c++11")
else()
  # Check whether we're using Intel compiler, which is generally
  # produces slightly faster code.
  #
  # To use Intel compiler, call cmake as: cmake -DCMAKE_CXX_COMPILER=icpc ..
  if (CMAKE_CXX_COMPILER_ID MATCHES Intel)
    set(CMAKE_CXX_FLAGS "${MORPH_HOST_DEFINITION} -Wall -g -std=c++11 -xHOST -O3 -qopenmp -D__ICC__")
    # No optimisation:
    #set(CMAKE_CXX_FLAGS "${MORPH_HOST_DEFINITION} -Wall -g -std=c++11 -xHOST -O0  -D__ICC__")
  else() # GCC
    set(CMAKE_CXX_FLAGS "${MORPH_HOST_DEFINITION} -Wall -g -std=c++11 -Wno-unused-result -march=native -O3 -fopenmp -D__GCC__")
  endif()
endif()

set (LIB_TYPE SHARED) # STATIC or SHARED
string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)
#find_package (HDF5) # Find non-cmake built HDF5, as installed on Ubuntu
# In case the above fails, something like this might work
find_package (HDF5 NAMES hdf5 COMPONENTS C ${SEARCH_TYPE})

include_directories(${HDF5_INCLUDE_DIR})

if (CMAKE_CXX_COMPILER_ID MATCHES Intel)
  # Intel tool debugging - ittnotify.h
  include_directories ("/opt/intel/vtune_amplifier_2019.0.2.570779/include")
endif()

# The Luschny factorial code uses the mpir library, which I obtained
# from github: git://github.com/wbhart/mpir.git (see also http://mpir.org)
find_library(mpir mpir PATH /usr/local/lib)
include_directories ("/usr/local/include") # for mpir.h

# Peter Luschny's factorial computation code. Compiles a library. In
# fact, I only ended up using the wrappers around mpir present in
# Peter's code, as it turns out the factorials that I need to compute
# and the n choose k's that I'm interested in are quite small and
# within the capabilities of mpir.
add_subdirectory(factorial)
include_directories (${CMAKE_SOURCE_DIR}/factorial)

include_directories (${CMAKE_SOURCE_DIR}/sim)
include_directories (${CMAKE_SOURCE_DIR}/sim/include)
# Main results in sim/
add_subdirectory(sim)
# Supplementary analysis in sim_supp/
add_subdirectory(sim_supp)

# Unit testing using the ctest framework
enable_testing()
add_subdirectory(tests)

# For debugging of variables:
set(DEBUG_VARIABLES OFF)
if(DEBUG_VARIABLES)
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endif(DEBUG_VARIABLES)
