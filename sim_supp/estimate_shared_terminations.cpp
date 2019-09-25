/*
 * Estimate the proportion of genomes for which both anterior and
 * posterior initial states will end on the same limit cycle - those
 * genomes which have 'shared terminations'.
 *
 * Author: S James
 * Date: November 2018.
 */

#include <iostream>
#include <vector>
#include <set>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// Choose debugging level. If you uncomment one of these, then you
// should probably reduce N_Generations
//
// #define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// The number of genomes to generate
#define N_Genomes 1000000

#include "lib.h"
#include "fitness.h"
#include "basins.h"

int main (int argc, char** argv)
{
    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);
    // Set up the Mersenne Twister RNG
    rngDataInit (&rd);
    zigset (&rd, DUMMYARG);
    rd.seed = seed;

    // Initialise masks
    masks_init();

    unsigned int num_sharedterm  = 0;

    array<genosect_t, N_Genes> genome;
    for (unsigned int i = 0; i < N_Genomes; ++i) {
        random_genome (genome);
        //double f = evaluate_fitness (genome);
        AllBasins ab (genome);
        BasinOfAttraction ba = ab.find (initial_ant);
        BasinOfAttraction bp = ab.find (initial_pos);
        if (ba.endpoint != ENDPOINT_UNKNOWN && (ba.limitCycle == bp.limitCycle)) {
            // Both initial states land on the same attractor
            ++num_sharedterm;
        }
    }

    LOG (num_sharedterm << " out of " << N_Genomes << " have the anterior and posterior state arriving on the same limit cycle.");

    return 0;
}
