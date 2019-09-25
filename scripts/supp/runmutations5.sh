#!/bin/bash

source script_common.sh

# Run several mutations in parallel.
./${HN}/sim_supp/mutation5 01 &
./${HN}/sim_supp/mutation5 02 &
./${HN}/sim_supp/mutation5 03 &
./${HN}/sim_supp/mutation5 04 &
./${HN}/sim_supp/mutation5 05 &
./${HN}/sim_supp/mutation5 06 &
./${HN}/sim_supp/mutation5 07 &
./${HN}/sim_supp/mutation5 08 &
./${HN}/sim_supp/mutation5 09 &
./${HN}/sim_supp/mutation5 10 &

wait
popd
