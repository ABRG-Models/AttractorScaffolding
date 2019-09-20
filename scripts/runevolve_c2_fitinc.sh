#!/bin/bash

#
# Fitness increment data generation
#

source script_common.sh
echo "Using build directory ${HN} for executables"

cat <<EOF > configs/runevolve_c2_fitinc.json
{
    "save_gensplus":     true,
    "logdir":       "./data/fitinc",
    "nGenerations":        10000000,
    "pOn":                      0.1,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "01010" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_fitinc.json 0.1 &
./${HN}/sim/evolve_j configs/runevolve_c2_fitinc.json 0.2 &
./${HN}/sim/evolve_j configs/runevolve_c2_fitinc.json 0.3 &
./${HN}/sim/evolve_j configs/runevolve_c2_fitinc.json 0.4 &
./${HN}/sim/evolve_j configs/runevolve_c2_fitinc.json 0.5 &

wait
popd

exit 0
