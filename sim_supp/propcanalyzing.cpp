/*
 * Find the proportion of genomes that have non-zero canalyzingness.
 *
 * This defaults the N_Genes=5, but N_Genes can be defined on the
 * compiler command line to find the results for other values.
 *
 * Author: S James
 * Date: September 2019.
 */

#include <iostream>
#include <vector>
#include <set>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <map>

using namespace std;

// Choose debugging level.
//
// #define DEBUGF 1
// #define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// Common code
#include "lib.h"

#include <climits>

int main (int argc, char** argv)
{
    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);

    // Initialise masks
    masks_init();

    // Unused, but set, in this program.
    pOn = 0.5;

    // To store the results of this program
    map<unsigned int, unsigned int> canalvalues;

    unsigned int ntrials = 10000000;

    array<genosect_t, N_Genes > genome;
    for (unsigned int i = 0; i < ntrials; ++i) {
        random_genome (genome);
        unsigned int c = canalyzingness(genome);
        if (c > 0) {
            cout << "." << flush;
        }
        if ((i % 100000) == 0) {
            cout << i << flush;
        }
        canalvalues[c] += 1;
    }

    // Output results
    cout << "Number of genomes tested: " << ntrials << endl;
    auto m = canalvalues.begin();
    while (m != canalvalues.end()) {
        cout << "Canalyzation value " << m->first << " seen " << m->second << " times" << endl;
        ++m;
    }

    return 0;
}
