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

TARG_ANT=24 # 0x18; 11000
TARG_POS=2  # 0x2; 00010

# Run several evolves in parallel with alternative targets
./${HN}/sim/evolve_ff0 0.05 ${TARG_ANT} ${TARG_POS} &
./${HN}/sim/evolve_ff0 0.1  ${TARG_ANT} ${TARG_POS} &
./${HN}/sim/evolve_ff0 0.15 ${TARG_ANT} ${TARG_POS} &
./${HN}/sim/evolve_ff0 0.2  ${TARG_ANT} ${TARG_POS} &
./${HN}/sim/evolve_ff0 0.25 ${TARG_ANT} ${TARG_POS} &
./${HN}/sim/evolve_ff0 0.3  ${TARG_ANT} ${TARG_POS} &
./${HN}/sim/evolve_ff0 0.35 ${TARG_ANT} ${TARG_POS} &
./${HN}/sim/evolve_ff0 0.4  ${TARG_ANT} ${TARG_POS} &
./${HN}/sim/evolve_ff0 0.45 ${TARG_ANT} ${TARG_POS} &
./${HN}/sim/evolve_ff0 0.5  ${TARG_ANT} ${TARG_POS} &

wait
popd
