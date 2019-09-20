#!/bin/bash

#
# Fitness increment data generation
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

cat <<EOF > configs/runevolve_c2_fitinc.json
{
    "save_gensplus":     true,
    "logdir":       "./data/fitinc",
    "nGenerations":        10000000,
    "pOn":                      0.1,
    "initial": [ "10000", "00000" ],
    "target":  [ "10101", "01010" ]
}
EOF
./${HN}/sim/evolve_j configs/runevolve_c2_fitinc.json 0.1 &
./${HN}/sim/evolve_j configs/runevolve_c2_fitinc.json 0.2 &
./${HN}/sim/evolve_j configs/runevolve_c2_fitinc.json 0.3 &
./${HN}/sim/evolve_j configs/runevolve_c2_fitinc.json 0.4 &
./${HN}/sim/evolve_j configs/runevolve_c2_fitinc.json 0.5 &

wait
popd

exit 0
