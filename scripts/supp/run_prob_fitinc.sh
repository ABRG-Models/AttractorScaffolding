#!/bin/bash

source script_common.sh
echo "Using build directory '${HN}' for executables"

# Run several evolves in parallel.
./${HN}/sim_supp/prob_fitinc 0.02 &
./${HN}/sim_supp/prob_fitinc 0.05 &
./${HN}/sim_supp/prob_fitinc 0.1 &
./${HN}/sim_supp/prob_fitinc 0.2 &
./${HN}/sim_supp/prob_fitinc 0.3 &
./${HN}/sim_supp/prob_fitinc 0.4 &
./${HN}/sim_supp/prob_fitinc 0.5 &

wait
popd
