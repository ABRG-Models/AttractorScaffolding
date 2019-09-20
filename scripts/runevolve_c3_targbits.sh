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

# Total of 4 hamming bits difference between targets
cat <<EOF > configs/runevolve_c3_targbits_4ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data_targbits",
    "nGenerations": 100000000,
    "pOn":                0.05,
    "initial": [ "01000", "00100", "00001" ],
    "target":  [ "10000", "00000", "00010" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c3_targbits_4ham.json &

# Total of 6 hamming bits difference between targets
cat <<EOF > configs/runevolve_c3_targbits_6ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data_targbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "01000", "00100", "00001" ],
    "target":  [ "10000", "00100", "00010" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c3_targbits_6ham.json &

# Total of 8 hamming bits difference between targets
cat <<EOF > configs/runevolve_c3_targbits_8ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data_targbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "01000", "00100", "00001" ],
    "target":  [ "10000", "00100", "00111" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c3_targbits_8ham.json &


# Total of 10 hamming bits difference between targets
cat <<EOF > configs/runevolve_c3_targbits_10ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data_targbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "01000", "00100", "00001" ],
    "target":  [ "10101", "01010", "00101" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c3_targbits_10ham.json &

# Total of 12 hamming bits difference between targets
cat <<EOF > configs/runevolve_c3_targbits_12ham.json
{
    "save_gensplus":     false,
    "logdir":        "./data_targbits",
    "nGenerations": 100000000,
    "pOn":                0.1,
    "initial": [ "10000", "00100", "00001" ],
    "target":  [ "10100", "01010", "00101" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c3_targbits_12ham.json &

wait
popd

exit 0
