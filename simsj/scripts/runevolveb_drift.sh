#!/bin/bash

PWD=`pwd | awk -F'/' '{ print $NF }'`
if [ ${PWD} = 'scripts' ]; then
    pushd ..
elif [ ${PWD} = 'simsj' ]; then # This line will change
    pushd .
else
    echo "Please run this from simsj or the scripts directory."
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
./${HN}/sim/evolve_bybits 14 &
./${HN}/sim/evolve_bybits 16 &
./${HN}/sim/evolve_bybits 18 &
./${HN}/sim/evolve_bybits 20 &

wait
popd

exit 0
