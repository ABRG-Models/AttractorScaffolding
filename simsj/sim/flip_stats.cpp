/*
 * Look at bit flipping stats.
 *
 * Author: S James
 * Date: Nov 2018.
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

// Choose debugging level. If you uncomment one of these, then you
// should probably reduce N_Generations
//
// #define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// The number of generations to evolve for. 20000000 takes roughly 30
// seconds on an Intel Core i9-8950HK CPU.
#ifndef N_Generations
# define N_Generations   100000000
#endif

// Common code
#include "lib.h"

// The fitness function used here
#include "fitness.h"

int main (int argc, char** argv)
{
    // Obtain pOn from command line.
    if (argc < 2) {
        LOG ("Usage: " << argv[0] << " pOn target_ant target_pos");
        LOG ("Supply the probability of flipping a gene during evolution, pOn (float, 0 to 1.0f)");
        LOG ("Optionally supply the anterior and posterior targets (integer, in range 0 to 31)");
        return 1;
    }
    pOn = static_cast<float>(atof (argv[1]));
    LOG ("Probability of flipping = " << pOn);

    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);

    // Initialise masks
    masks_init();

    // Reference genome
    array<genosect_t, N_Genes> refg;

    array<unsigned long long int, N_Genes> flipcount;
    for (unsigned int i = 0; i < N_Genes; ++i) {
        flipcount[i] = 0;
    }
    for (unsigned int i = 0; i < N_Generations; ++i) {
        evolve_genome (refg, flipcount);
    }
    cout << "For pOn = " << pOn << "..." << endl;
    for (int j = 0; j < N_Genes; ++j) {
        cout << "Mean number of bits flipped in genome section for gene " << j << " was " << flipcount[j] / (double)N_Generations << endl;
    }
    return 0;
}
