#!/bin/bash

source script_common.sh

# Run several mutations in parallel.
./${HN}/sim_supp/mutation4 01 &
./${HN}/sim_supp/mutation4 02 &
./${HN}/sim_supp/mutation4 03 &
./${HN}/sim_supp/mutation4 04 &
./${HN}/sim_supp/mutation4 05 &
./${HN}/sim_supp/mutation4 06 &
./${HN}/sim_supp/mutation4 07 &
./${HN}/sim_supp/mutation4 08 &
./${HN}/sim_supp/mutation4 09 &
./${HN}/sim_supp/mutation4 10 &

wait
popd
