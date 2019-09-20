#!/bin/bash

source script_common.sh
echo "Using build directory ${HN} for executables"

# Run several drifts in parallel.
./${HN}/sim_supp/drift 0.1 &
./${HN}/sim_supp/drift 0.2 &
./${HN}/sim_supp/drift 0.3 &
./${HN}/sim_supp/drift 0.4 &
./${HN}/sim_supp/drift 0.5 &

wait
popd
