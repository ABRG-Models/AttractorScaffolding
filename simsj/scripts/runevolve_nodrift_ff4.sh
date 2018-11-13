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
./${HN}/sim/evolve_nodrift_ff4 0.05 &
./${HN}/sim/evolve_nodrift_ff4 0.10 &
./${HN}/sim/evolve_nodrift_ff4 0.15 &
./${HN}/sim/evolve_nodrift_ff4 0.20 &
./${HN}/sim/evolve_nodrift_ff4 0.25 &
./${HN}/sim/evolve_nodrift_ff4 0.3 &
./${HN}/sim/evolve_nodrift_ff4 0.35 &
./${HN}/sim/evolve_nodrift_ff4 0.4 &
./${HN}/sim/evolve_nodrift_ff4 0.45 &
./${HN}/sim/evolve_nodrift_ff4 0.5 &

wait
popd
