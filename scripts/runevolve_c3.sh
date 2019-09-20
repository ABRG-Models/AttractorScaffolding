#!/bin/bash

#
# Evolve algo, n=5 genes, Target/Initial config from JSON
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

cat <<EOF > configs/runevolve_c3.json
{
    "save_gensplus":     false,
    "logdir":         "./data",
    "nGenerations": 1000000000,
    "pOn":                0.05,
    "initial": [ "10000", "00100", "00001" ],
    "target":  [ "10100", "00101", "01010" ]
}
EOF

# Run several evolve_js in parallel.
./${HN}/sim/evolve_j configs/runevolve_c3.json 0.03 &
./${HN}/sim/evolve_j configs/runevolve_c3.json 0.05 &
./${HN}/sim/evolve_j configs/runevolve_c3.json 0.10 &
./${HN}/sim/evolve_j configs/runevolve_c3.json 0.15 &
./${HN}/sim/evolve_j configs/runevolve_c3.json 0.20 &
./${HN}/sim/evolve_j configs/runevolve_c3.json 0.3  &
./${HN}/sim/evolve_j configs/runevolve_c3.json 0.4  &
./${HN}/sim/evolve_j configs/runevolve_c3.json 0.5  &

wait
popd

exit 0
