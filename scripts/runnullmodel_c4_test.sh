#!/bin/bash

#
# Run the null model - setting p to 0.5 - for long enough to get stats
# about random discovery rate.
#

source script_common.sh
echo "Using build directory ${HN} for executables"

NCPU=`nproc`
NFIT=64
NFIT_PERPROC=$((NFIT / NCPU))
echo "N fit to find per process: ${NFIT_PERPROC}"

PON="0.4"

for i in $(seq 1 ${NCPU}); do

    mkdir -p ./data/null/${i}

    cat <<EOF > configs/runnull_c4_${i}.json
{
    "save_gensplus":       false,
    "logdir":           "./data/null/${i}",
    "finishAfterNFit":         ${NFIT_PERPROC},
    "pOn":                   ${PON}, // debug
    "initial": [ "10000", "00100", "00001", "01000" ],
    "target":  [ "10100", "01010", "00101", "01001" ]
}
EOF
done

for i in $(seq 1 ${NCPU}); do
    # On each processor, run over and over, 1 times
    sleep 1 && ./${HN}/sim/evolve configs/runnull_c4_${i}.json &
done

wait
popd

# Collect results
DFILE=../data/combined_${PON}.csv
for i in $(seq 1 ${NCPU}); do
    cat ../data/null/${i}/evolve_nc4_I16-4-1-8_T20-10-5-9_ff4_${NFIT_PERPROC}_fits_${PON}.csv >> ${DFILE}
done

exit 0
