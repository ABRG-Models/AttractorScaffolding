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
./${HN}/sim/mutation6 01 &
./${HN}/sim/mutation6 02 &
./${HN}/sim/mutation6 03 &
./${HN}/sim/mutation6 04 &
./${HN}/sim/mutation6 05 &
./${HN}/sim/mutation6 06 &
./${HN}/sim/mutation6 07 &
./${HN}/sim/mutation6 08 &
./${HN}/sim/mutation6 09 &
./${HN}/sim/mutation6 10 &

wait
popd
