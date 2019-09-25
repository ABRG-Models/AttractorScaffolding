#!/bin/bash

source script_common.sh

# Run several mutations in parallel.
./${HN}/sim_supp/mutation6 01 &
./${HN}/sim_supp/mutation6 02 &
./${HN}/sim_supp/mutation6 03 &
./${HN}/sim_supp/mutation6 04 &
./${HN}/sim_supp/mutation6 05 &
./${HN}/sim_supp/mutation6 06 &
./${HN}/sim_supp/mutation6 07 &
./${HN}/sim_supp/mutation6 08 &
./${HN}/sim_supp/mutation6 09 &
./${HN}/sim_supp/mutation6 10 &

wait
popd
