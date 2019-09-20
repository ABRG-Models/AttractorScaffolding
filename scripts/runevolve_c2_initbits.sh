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
