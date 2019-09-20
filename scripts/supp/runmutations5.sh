#!/bin/bash

source script_common.sh

# Run several mutations in parallel.
./${HN}/sim/mutation5 01 &
./${HN}/sim/mutation5 02 &
./${HN}/sim/mutation5 03 &
./${HN}/sim/mutation5 04 &
./${HN}/sim/mutation5 05 &
./${HN}/sim/mutation5 06 &
./${HN}/sim/mutation5 07 &
./${HN}/sim/mutation5 08 &
./${HN}/sim/mutation5 09 &
./${HN}/sim/mutation5 10 &

wait
popd
