#!/bin/bash

PWD=`pwd | awk -F'/' '{ print $NF }'`
if [ ${PWD} = 'scripts' ]; then
    pushd ..
elif [ ${PWD} = 'AttractorScaffolding' ]; then # This line will change
    pushd .
else
    echo "Please run this from AttractorScaffolding or the scripts directory."
fi

# Find out where to run from.
HN=`hostname`
if [ ! -d ${HN} ]; then
    if [ -d build ]; then
        echo "No directory '${HN}'; defaulting to the directory 'build'"
        HN=build
    else
        echo "Please build the software using cmake in a directory called '${HN}' or 'build'."
        exit 1
    fi
fi

echo "Using build directory ${HN} for executables"

# Run several evolves in parallel.
./${HN}/sim/evolve_bybits 2  &
./${HN}/sim/evolve_bybits 3  &
./${HN}/sim/evolve_bybits 4  &
./${HN}/sim/evolve_bybits 5  &
./${HN}/sim/evolve_bybits 6  &
./${HN}/sim/evolve_bybits 8  &
./${HN}/sim/evolve_bybits 10 &
./${HN}/sim/evolve_bybits 12 &
./${HN}/sim/evolve_bybits 16 &
./${HN}/sim/evolve_bybits 20 &
./${HN}/sim/evolve_bybits 24  &
./${HN}/sim/evolve_bybits 28  &
./${HN}/sim/evolve_bybits 32  &
./${HN}/sim/evolve_bybits 36  &
./${HN}/sim/evolve_bybits 40  &

wait
popd

exit 0
