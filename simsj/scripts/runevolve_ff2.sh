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
./${HN}/sim/evolve_ff2 0.05 &
./${HN}/sim/evolve_ff2 0.07 &
./${HN}/sim/evolve_ff2 0.10 &
./${HN}/sim/evolve_ff2 0.13 &
./${HN}/sim/evolve_ff2 0.15 &
./${HN}/sim/evolve_ff2 0.17 &
./${HN}/sim/evolve_ff2 0.19 &
./${HN}/sim/evolve_ff2 0.21 &
./${HN}/sim/evolve_ff2 0.5 &

wait
popd
