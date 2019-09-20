#!/bin/bash

source script_common.sh
echo "Using build directory ${HN} for executables"

# Run several in parallel.
./${HN}/sim_supp/compute_pnot0 3 &
./${HN}/sim_supp/compute_pnot0 4 &
./${HN}/sim_supp/compute_pnot0 5 &
./${HN}/sim_supp/compute_pnot0 6 &
./${HN}/sim_supp/compute_pnot0 7 &
./${HN}/sim_supp/compute_pnot0 9 &
./${HN}/sim_supp/compute_pnot0 12 &
./${HN}/sim_supp/compute_pnot0 15 &
./${HN}/sim_supp/compute_pnot0 18 &
./${HN}/sim_supp/compute_pnot0 25 &

wait
popd
