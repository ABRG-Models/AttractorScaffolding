#!/bin/bash

source script_common.sh
echo "Using build directory ${HN} for executables"

pwd
${HN}/sim_supp/h_m 4 > data/h4_m.csv
${HN}/sim_supp/h_m 5 > data/h5_m.csv
${HN}/sim_supp/h_m 6 > data/h6_m.csv

wait
popd
