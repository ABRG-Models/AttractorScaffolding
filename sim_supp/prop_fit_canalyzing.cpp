/*
 * Find the proportion of fit genomes which are canalyzing functions.
 *
 * Author: S James
 * Date: September 2019.
 */

#include <iostream>
#include <map>
#include <array>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// Choose debugging level.
//
//#define DEBUGF 1
//#define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// Common code
#include "lib.h"

// The fitness function used here
#include "fitness.h"
#include "mutation.h"

int main (int argc, char** argv)
{
    // Seed the RNG
#define PROPERLY_RANDOM_SEED
#ifdef PROPERLY_RANDOM_SEED
    unsigned int seed = mix(clock(), time(NULL), getpid());
#else
    unsigned int seed = 4;
#endif
    srand (seed);

    // Make the evolutions as fast as possible
    pOn = 0.05;

    // Initialise masks
    masks_init();

    // To store the results of this program
    map<unsigned int, unsigned int> canalvalues;

    unsigned int ntrials = 100000;

    array<genosect_t, N_Genes > genome;

    for (unsigned int i = 0; i < ntrials; ++i) {
        //cout << "+" << flush;
        genome = evolve_new_genome ();
        random_genome (genome);
        unsigned int c = canalyzingness(genome);
        if (c > 0) {
            cout << "." << flush;
        }
        if ((i % 10000) == 0) {
            cout << i << flush;
        }
        canalvalues[c] += 1;
    }

    // Output results
    cout << "Number of fit genomes tested: " << ntrials << endl;
    auto m = canalvalues.begin();
    while (m != canalvalues.end()) {
        cout << "Canalyzation value " << m->first << " seen " << m->second << " times" << endl;
        ++m;
    }

    return 0;
}
