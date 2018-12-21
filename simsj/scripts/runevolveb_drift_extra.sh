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
./${HN}/sim/evolve_bybits 22  &
./${HN}/sim/evolve_bybits 24  &
./${HN}/sim/evolve_bybits 26  &
./${HN}/sim/evolve_bybits 28  &
./${HN}/sim/evolve_bybits 30  &
./${HN}/sim/evolve_bybits 32  &
./${HN}/sim/evolve_bybits 34  &
./${HN}/sim/evolve_bybits 36  &
./${HN}/sim/evolve_bybits 38  &
./${HN}/sim/evolve_bybits 40  &

wait
popd

exit 0
