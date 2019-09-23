#!/bin/bash

#
# Evolve algo, n=5 genes, Target/Initial config from JSON. 3
# contexts. Explore effect of differences in target bits.
#

source script_common.sh
echo "Using build directory ${HN} for executables"

# 5 bits
cat <<EOF > configs/runevolve_c2_targbits_5ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/targbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "01010" ]
}
EOF
./${HN}/sim/evolve configs/runevolve_c2_targbits_5ham.json &

# 4 bits
cat <<EOF > configs/runevolve_c2_targbits_4ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/targbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "11010" ]
}
EOF
./${HN}/sim/evolve configs/runevolve_c2_targbits_4ham.json &

# 3 bits
cat <<EOF > configs/runevolve_c2_targbits_3ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/targbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "11110" ]
}
EOF
./${HN}/sim/evolve configs/runevolve_c2_targbits_3ham.json &

# 2 bits
cat <<EOF > configs/runevolve_c2_targbits_2ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/targbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "11111" ]
}
EOF
./${HN}/sim/evolve configs/runevolve_c2_targbits_2ham.json &

# 1 bits
cat <<EOF > configs/runevolve_c2_targbits_1ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/targbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "10111" ]
}
EOF
./${HN}/sim/evolve configs/runevolve_c2_targbits_1ham.json &


wait
popd

exit 0
