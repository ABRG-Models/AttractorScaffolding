/*
 * Mutate a known-good genome and cout the strings.
 *
 * Author: S James
 * Date: October 2018.
 */

#include <iostream>
#include <vector>
#include <set>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

// Choose debugging level.
//
// #define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// Common code
#include "lib.h"

// The fitness function used here
#include "fitness.h"

int main (int argc, char** argv)
{
    // Seed the RNG
    unsigned int seed = 4;
    srand (seed);
    // Set up the Mersenne Twister RNG
    rngDataInit (&rd);
    zigset (&rd, DUMMYARG);
    rd.seed = seed;

    // Initialise masks
    masks_init();

    pOn = 0.05;

    // Holds the genome
    array<genosect_t, N_Genes> genome = selected_genome();
    unsigned int i = 0;
    while (i++ < 100) {
        cout << genome2str (genome) << endl;
        evolve_genome (genome);
    }

    return 0;
}
