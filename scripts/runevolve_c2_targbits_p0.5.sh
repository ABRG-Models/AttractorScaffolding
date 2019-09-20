#!/bin/bash

#
# Evolve algo, n=5 genes, Target/Initial config from JSON. 3
# contexts. Explore effect of differences in target bits.
#

PWD=`pwd | awk -F'/' '{ print $NF }'`
if [ ${PWD} = 'scripts' ]; then
    pushd ..
elif [ ${PWD} = 'AttractorScaffolding' ]; then # This line will change
    pushd .
else
    echo "Please run this from AttractorScaffolding or the scripts directory."
fi

# Find out where to run from.
HN=`hostname`
if [ ! -d ${HN} ]; then
    if [ -d build ]; then
        echo "No directory '${HN}'; defaulting to the directory 'build'"
        HN=build
    else
        echo "Please build the software using cmake in a directory called '${HN}' or 'build'."
        exit 1
    fi
fi

echo "Using build directory ${HN} for executables"

# 5 bits
cat <<EOF > configs/runevolve_c2_targbits_5ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/targbits",
    "nGenerations": 100000000,
    "pOn":                0.5,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "01010" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_targbits_5ham.json &

# 4 bits
cat <<EOF > configs/runevolve_c2_targbits_4ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/targbits",
    "nGenerations": 100000000,
    "pOn":                0.5,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "11010" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_targbits_4ham.json &

# 3 bits
cat <<EOF > configs/runevolve_c2_targbits_3ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/targbits",
    "nGenerations": 100000000,
    "pOn":                0.5,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "11110" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_targbits_3ham.json &

# 2 bits
cat <<EOF > configs/runevolve_c2_targbits_2ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/targbits",
    "nGenerations": 100000000,
    "pOn":                0.5,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "11111" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_targbits_2ham.json &

# 1 bits
cat <<EOF > configs/runevolve_c2_targbits_1ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data/targbits",
    "nGenerations": 100000000,
    "pOn":                0.5,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "10111" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_targbits_1ham.json &


wait
popd

exit 0
