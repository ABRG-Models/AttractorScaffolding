#!/bin/bash

#
# Evolve algo, n=5 genes, Target/Initial config from JSON
#

PWD=`pwd | awk -F'/' '{ print $NF }'`
if [ ${PWD} = 'scripts' ]; then
    pushd ..
elif [ ${PWD} = 'AttractorScaffolding' ]; then # This line will change
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

# Run several evolve_js in parallel.
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.01 &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.02 &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.03 &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.04 &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.05 &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.10 &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.15 &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.20 &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.25 &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.3  &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.35 &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.4  &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.45 &
./${HN}/sim/evolve_j configs/evolve_three_1.json 0.5  &

wait
popd

exit 0
