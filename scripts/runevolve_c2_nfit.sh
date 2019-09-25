#!/bin/bash

#
# Evolve algo, n=5 genes, Target/Initial config from JSON. Reproduces
# the main figures in the paper, esp. fig 4.
#
# Runs best on a CPU with 16 cores or more.
#

source script_common.sh
echo "Using build directory ${HN} for executables"

cat <<EOF > configs/runevolve_c2.json
{
    "save_gensplus":     false,
    "logdir":         "./data",
    "nGenerations":  100000000,
    "finishAfterNFit":    1000,
    "pOn":                0.05,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "01010" ]
}
EOF

# Run several evolves in parallel. 5 results shown in fig 4.
./${HN}/sim/evolve configs/runevolve_c2.json 0.1 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.2 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.3 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.4 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.5 &

# These are the extra data points for fig 6:
./${HN}/sim/evolve configs/runevolve_c2.json 0.02 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.03 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.04 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.05 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.07 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.12 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.15 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.17 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.25 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.35 &
./${HN}/sim/evolve configs/runevolve_c2.json 0.45 &

wait
popd

exit 0
