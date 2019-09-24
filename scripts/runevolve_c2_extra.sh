#!/bin/bash

#
# Evolve algo, n=5 genes, Target/Initial config from JSON
#

source script_common.sh
echo "Using build directory ${HN} for executables"

cat <<EOF > configs/runevolve_c2.json
{
    "save_gensplus":     false,
    "logdir":         "./data",
    "nGenerations":  100000000,
    "pOn":                0.05,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "01010" ]
}
EOF

# Extra:
./${HN}/sim/evolve configs/runevolve_c2.json 0.02 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.04 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.07 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.12 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.17 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.25 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.35 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.45 &

wait
popd

exit 0
