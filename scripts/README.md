# Scripts to run several simulation programs in parallel

### runcomplexity.sh

Runs analysis of complexity of random and fit genomes. See
sim/complexity_random.cpp and sim/complexity_fit.cpp.

### runevolve_c2.sh

Runs the 2 context (anterior/posterior) model which is the main result
of the paper.

### runevolve_c2_async.sh

Runs the 2 context system with asynchronous, rather than synchronous
state updating.

### runevolve_c2_initbits.sh

Runs the 2 context system five times with p=0.1, varying the Hamming
distance between the 2 initial states of the system.

### runevolve_c2_targbits.sh

Runs the 2 context system five times with p=0.1, varying the Hamming
distance between the 2 target states of the system.

### runevolve_c2_targbits_p0.5.sh

Runs the 2 context system five times with p=0.5, varying the Hamming
distance between the 2 target states of the system.

### runevolve_c2_fitinc.sh

Runs the 2 context system, recording information about fitness
increments, as well as the information about f=1 genomes discovered.

### runevolve_c2_withf.sh

Runs the 2 context system, recording information about the state space
(number of basins, etc), the fitness increments, as well as the
information about f=1 genomes discovered.

### runevolve_c3.sh

Runs the 3 context model.

### runevolve_c3_targbits.sh

Runs the 3 context system five times with p=0.1, varying the summed
Hamming distance between the 3 target states of the system.

### runevolve_c4.sh

Runs the 4 context model.

### runnullmodel_c3.sh

Runs evolve for 3 contexts with p=0.5, executing for long enough to
get good statistics on the number of generations on average that are
required to find an f=1 genome.

### runnullmodel_c4.sh

Runs evolve for 4 contexts with p=0.5, executing for long enough to
get good statistics on the number of generations on average that are
required to find an f=1 genome. This script has to split the
processing up into many separate processes. We ran it on a 32 core
Threadripper processor, requiring about 3-4 hours to complete.
