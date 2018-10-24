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

# Run several mutations in parallel.
./${HN}/sim/mutation4_ff4 01 &
./${HN}/sim/mutation4_ff4 02 &
./${HN}/sim/mutation4_ff4 03 &
./${HN}/sim/mutation4_ff4 04 &
./${HN}/sim/mutation4_ff4 05 &
./${HN}/sim/mutation4_ff4 06 &
./${HN}/sim/mutation4_ff4 07 &
./${HN}/sim/mutation4_ff4 08 &
./${HN}/sim/mutation4_ff4 09 &
./${HN}/sim/mutation4_ff4 10 &

wait
popd
