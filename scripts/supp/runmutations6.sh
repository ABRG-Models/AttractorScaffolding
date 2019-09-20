#!/bin/bash

source script_common.sh

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
