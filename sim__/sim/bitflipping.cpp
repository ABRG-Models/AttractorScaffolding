/*
 * Randomly generate a genome. Look at the number of changed
 * transitions for flipping each single bit.
 *
 * Author: S James
 * Date: December 2018.
 */

#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// Choose debugging level.
//
#define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// Common code
#include "lib.h"

#include "basins.h"
#include <algorithm>

// The fitness function used here
#include "fitness.h"

int main (int argc, char** argv)
{
    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);

    // Initialise masks
    masks_init();

    // Holds the original genome
    array<genosect_t, N_Genes> genome;
    // Holds the new, bitflipped genome
    array<genosect_t, N_Genes> genome1;

    double f = 0.0;
    double lastf = 0.0;
    AllBasins ab;
    AllBasins ab1;

    // The starting genome
    genome = random_genome();

    lastf = evaluate_fitness (genome);
    DBG ("Fitness of unflipped genome = " << lastf);
    ab.update (genome);

    for (unsigned int g = 0; g < N_Genes; ++g) {
        for (unsigned int i = 0; i < (1<<N_Ins); ++i) {

            // Bit flip genome[i*j]. Copy original first.
            copy_genome (genome, genome1);

            bitflip_genome (genome1, g, i);

            f = evaluate_fitness (genome1);

            ab1.update (genome1);
#ifdef DEBUG2
            // Get difference between sets
            // Find the difference in the transitions defined by
            // the two different genomes
            unsigned int count = 0;
            for (auto abr : ab.transitions) {
                LOG (count++ << ": " << hex << abr << dec);
            }
            LOG("---");
            count = 0;
            for (auto abr : ab1.transitions) {
                LOG (count++ << ": " << hex << abr << dec);
            }
#endif
            set<unsigned int> difference;
            set_difference (ab.transitions.begin(), ab.transitions.end(),
                            ab1.transitions.begin(), ab1.transitions.end(),
                            inserter(difference, difference.begin()));
            LOG ("Num changed transitions: " <<  difference.size() << ", fitness: " << f << "\t\tdelta_f: " << (f-lastf));
        }
    }

    return 0;
}
