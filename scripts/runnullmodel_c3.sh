#!/bin/bash

#
# Run the null model - setting p to 0.5 - for long enough to get stats
# about random discovery rate for the 3 context model. Carry out 5e9
# trials.
#

source script_common.sh
echo "Using build directory ${HN} for executables"

cat <<EOF > configs/runnull_c3.json
{
    "save_gensplus":     false,
    "logdir":         "./data",
    "nGenerations": 5000000000,
    "pOn":                0.5,
    "initial": [ "10000", "00100", "00001" ],
    "target":  [ "10100", "00101", "01010" ]
}
EOF

# Run several evolve_js in parallel.
./${HN}/sim/evolve_j configs/runnull_c3.json & # Repeat 3 contexts with more generations

wait
popd

exit 0
