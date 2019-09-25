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
    "finishAfterNFit":    1000,
    "pOn":                  0.05,
    "initial": [ "10000", "00100", "00001", "01000" ],
    "target":  [ "10100", "01010", "00101", "01001" ]
}
EOF

cat <<EOF > configs/runevolve_c4_slow1.json
{
    "save_gensplus":       false,
    "logdir":           "./data",
    "finishAfterNFit":       400,
    "pOn":                  0.05,
    "initial": [ "10000", "00100", "00001", "01000" ],
    "target":  [ "10100", "01010", "00101", "01001" ]
}
EOF

cat <<EOF > configs/runevolve_c4_slow2.json
{
    "save_gensplus":       false,
    "logdir":           "./data",
    "finishAfterNFit":     30,
    "pOn":                  0.05,
    "initial": [ "10000", "00100", "00001", "01000" ],
    "target":  [ "10100", "01010", "00101", "01001" ]
}
EOF

# Run several evolves in parallel.
./${HN}/sim/evolve configs/runevolve_c4.json 0.02 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.03 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.04 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.05 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.07 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.10 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.12 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.15 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.17 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.20 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.25 &
./${HN}/sim/evolve configs/runevolve_c4.json 0.3  &
./${HN}/sim/evolve configs/runevolve_c4.json 0.35 &
# These provide poor statistical data

# slow
./${HN}/sim/evolve configs/runevolve_c4_slow1.json 0.4  &
./${HN}/sim/evolve configs/runevolve_c4_slow1.json 0.45 &

# Crazy slow. 50 hits could take all day
./${HN}/sim/evolve configs/runevolve_c4_slow2.json 0.5  &

wait
popd

exit 0
