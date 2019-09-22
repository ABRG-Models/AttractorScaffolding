#!/bin/bash

#
# Run the null model - setting p to 0.5 - for long enough to get stats
# about random discovery rate.
#

source script_common.sh
echo "Using build directory ${HN} for executables"

NCPU=`nproc`
#NCPU=32

NGEN=100000000000
#NGEN=9999999 # for debug
NGEN_PERPROC=$((NGEN / NCPU))
echo "N gen per process: ${NGEN_PERPROC}"
for i in $(seq 1 ${NCPU}); do

    mkdir -p ./data/null/${i}

    cat <<EOF > configs/runnull_c4_${i}.json
{
    "save_gensplus":       false,
    "logdir":           "./data/null/${i}",
    "nGenerations":  ${NGEN_PERPROC},
    "pOn":                   0.5,
    "initial": [ "10000", "00100", "00001", "01000" ],
    "target":  [ "10100", "01010", "00101", "01001" ]
}
EOF

    sleep 1 && ./${HN}/sim/evolve_j configs/runnull_c4_${i}.json &
done

wait
popd

# Collect results
DFILE=../data/evolve_nc4_I16-4-1-8_T20-10-5-9_ff4_${NGEN}_gens_0.5.csv
#if [ -f ${DFILE} ]; then
#    rm -f ${DFILE}
#fi
for i in $(seq 1 ${NCPU}); do
    cat ../data/null/${i}/evolve_nc4_I16-4-1-8_T20-10-5-9_ff4_${NGEN_PERPROC}_gens_0.5.csv >> ${DFILE}
done

exit 0
