#!/bin/bash

source script_common.sh
echo "Using build directory ${HN} for executables"

# Run several evolves in parallel.
./${HN}/sim_supp/evolve_bybits 2  &
./${HN}/sim_supp/evolve_bybits 3  &
./${HN}/sim_supp/evolve_bybits 4  &
./${HN}/sim_supp/evolve_bybits 5  &
./${HN}/sim_supp/evolve_bybits 6  &
./${HN}/sim_supp/evolve_bybits 8  &
./${HN}/sim_supp/evolve_bybits 10 &
./${HN}/sim_supp/evolve_bybits 12 &
./${HN}/sim_supp/evolve_bybits 16 &
./${HN}/sim_supp/evolve_bybits 20 &
./${HN}/sim_supp/evolve_bybits 24  &
./${HN}/sim_supp/evolve_bybits 28  &
./${HN}/sim_supp/evolve_bybits 32  &
./${HN}/sim_supp/evolve_bybits 36  &
./${HN}/sim_supp/evolve_bybits 40  &

wait
popd

exit 0
