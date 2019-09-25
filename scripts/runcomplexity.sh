#!/bin/bash

#
# Run the complexity analysis for both random and known fit genomes
#

source script_common.sh
echo "Using build directory ${HN} for executables"

./${HN}/sim/complexity_random &
./${HN}/sim/complexity_fit &

wait

popd

exit 0
