#!/bin/bash

source script_common.sh
echo "Using build directory ${HN} for executables"

# Run several evolves in parallel.
./${HN}/sim_supp/prob_fitinc_bybits 3 &
./${HN}/sim_supp/prob_fitinc_bybits 4 &
./${HN}/sim_supp/prob_fitinc_bybits 5 &
./${HN}/sim_supp/prob_fitinc_bybits 6 &
./${HN}/sim_supp/prob_fitinc_bybits 8 &
./${HN}/sim_supp/prob_fitinc_bybits 10 &
./${HN}/sim_supp/prob_fitinc_bybits 15 &
./${HN}/sim_supp/prob_fitinc_bybits 20 &

wait
popd
