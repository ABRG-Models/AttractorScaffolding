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
./${HN}/sim/prob_fitinc_bybits 3 &
./${HN}/sim/prob_fitinc_bybits 4 &
./${HN}/sim/prob_fitinc_bybits 5 &
./${HN}/sim/prob_fitinc_bybits 6 &
./${HN}/sim/prob_fitinc_bybits 8 &
./${HN}/sim/prob_fitinc_bybits 10 &
./${HN}/sim/prob_fitinc_bybits 15 &
./${HN}/sim/prob_fitinc_bybits 20 &

wait
popd
