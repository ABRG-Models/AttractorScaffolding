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

# Run several in parallel.
./${HN}/sim/compute_pnot0 3 &
./${HN}/sim/compute_pnot0 4 &
./${HN}/sim/compute_pnot0 5 &
./${HN}/sim/compute_pnot0 6 &
./${HN}/sim/compute_pnot0 7 &
./${HN}/sim/compute_pnot0 9 &
./${HN}/sim/compute_pnot0 12 &
./${HN}/sim/compute_pnot0 15 &
./${HN}/sim/compute_pnot0 18 &
./${HN}/sim/compute_pnot0 25 &

wait
popd
