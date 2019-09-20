#!/bin/bash

source script_common.sh

# Run several mutations in parallel.
./${HN}/sim/mutation4 01 &
./${HN}/sim/mutation4 02 &
./${HN}/sim/mutation4 03 &
./${HN}/sim/mutation4 04 &
./${HN}/sim/mutation4 05 &
./${HN}/sim/mutation4 06 &
./${HN}/sim/mutation4 07 &
./${HN}/sim/mutation4 08 &
./${HN}/sim/mutation4 09 &
./${HN}/sim/mutation4 10 &

wait
popd
