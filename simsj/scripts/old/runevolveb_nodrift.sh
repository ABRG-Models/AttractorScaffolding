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
./${HN}/sim/evolve_bybits_nodrift 2  &
./${HN}/sim/evolve_bybits_nodrift 3  &
./${HN}/sim/evolve_bybits_nodrift 4  &
./${HN}/sim/evolve_bybits_nodrift 5  &
./${HN}/sim/evolve_bybits_nodrift 6  &
./${HN}/sim/evolve_bybits_nodrift 8  &
./${HN}/sim/evolve_bybits_nodrift 10 &
./${HN}/sim/evolve_bybits_nodrift 12 &
./${HN}/sim/evolve_bybits_nodrift 14 &
./${HN}/sim/evolve_bybits_nodrift 16 &
./${HN}/sim/evolve_bybits_nodrift 18 &
./${HN}/sim/evolve_bybits_nodrift 20 &

wait
popd

exit 0
