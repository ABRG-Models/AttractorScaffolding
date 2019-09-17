#!/bin/bash

#
# Evolve algo, n=5 genes, 4 bits difference in initial state.
# Initial states 10000/01110 (Target states 10101 and 01010)
#

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

TA=21 # 10101
TP=10 # 01010

IA=16 # 10000
IP=14 # 01110

# Run several evolves in parallel.
# evolve pOn targ_ant targ_pos init_ant init_pos
./${HN}/sim/evolve 0.01 $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.02 $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.03 $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.04 $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.05 $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.10 $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.15 $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.20 $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.25 $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.3  $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.35 $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.4  $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.45 $TA $TP $IA $IP  &
./${HN}/sim/evolve 0.5  $TA $TP $IA $IP  &

wait
popd

exit 0
