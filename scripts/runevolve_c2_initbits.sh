#!/bin/bash

#
# Evolve algo, n=5 genes, Target/Initial config from JSON. 3
# contexts. Explore effect of differences in target bits.
#

source script_common.sh
echo "Using build directory ${HN} for executables"

# 5 bits
cat <<EOF > configs/runevolve_c2_initbits_5ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/initbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "10000", "01111" ],
    "target":  [ "10101", "01010" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_initbits_5ham.json &

# 4 bits
cat <<EOF > configs/runevolve_c2_initbits_4ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/initbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "10000", "00111" ],
    "target":  [ "10101", "01010" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_initbits_4ham.json &

# 3 bits
cat <<EOF > configs/runevolve_c2_initbits_3ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/initbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "10000", "00011" ],
    "target":  [ "10101", "01010" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_initbits_3ham.json &

# 2 bits
cat <<EOF > configs/runevolve_c2_initbits_2ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/initbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "10000", "00001" ],
    "target":  [ "10101", "01010" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_initbits_2ham.json &

# 1 bits
cat <<EOF > configs/runevolve_c2_initbits_1ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/initbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "01010" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_initbits_1ham.json &


wait
popd

exit 0
