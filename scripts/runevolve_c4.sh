#!/bin/bash

#
# Evolve algo, n=5 genes, Target/Initial config from JSON
#

source script_common.sh
echo "Using build directory ${HN} for executables"

cat <<EOF > configs/runevolve_c4.json
{
    "save_gensplus":       false,
    "logdir":           "./data",
    "nGenerations":   5000000000,
    "pOn":                  0.05,
    "initial": [ "10000", "00100", "00001", "01000" ],
    "target":  [ "10100", "01010", "00101", "01001" ]
}
EOF

# Run several evolves in parallel.
./${HN}/sim/evolve configs/runevolve_c4.json 0.03 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.05 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.10 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.15 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.20 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.3  &
./${HN}/sim/evolve configs/runevolve_c4.json 0.4  &
./${HN}/sim/evolve configs/runevolve_c4.json 0.5  &

wait
popd

exit 0
