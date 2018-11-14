/*
 * Find the proportion of fit genomes from a random selection.
 *
 * This defaults the N_Genes=5, but N_Genes can be defined on the
 * compiler command line to find the results for other values.
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
#include <math.h>
#include <sys/types.h>
#include <unistd.h>

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

// The fitness function used here
#include "fitness.h"

int main (int argc, char** argv)
{
    string mid = "";
    if (argc > 1) {
        // Mutation ID
        mid = string(argv[1]);
    }

    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);

    // Initialise masks
    masks_init();

    // Unused, but set, in this program.
    pOn = 0.5;

    // Dan's fitness function takes about 1 second per million. 2^32
    // will take about 4000 seconds or a bit over an hour.
    //
    // Stuart's fitness function takes more computation, requiring:
    // 7.5 seconds for a million (N_Genes=4).
    // 21 seconds for a million (N_Genes=5).
    // 65 seconds for a million (N_Genes=6).

    unsigned long long int nrandoms = 1000000; // 1ULL << 32;
    unsigned long long int numfit = 0;
    unsigned long long int numperfect = 0;
    cout << "iteration,numfit,numperfect" << endl;
    for (unsigned long long int g = 0; g < nrandoms; ++g) {
        array<genosect_t, N_Genes> genome = random_genome();
        double f = evaluate_fitness (genome);
        if (f > 0.0) {
            ++numfit;
            if (f == 1.0) {
                ++numperfect;
            }
        }
        if (g%1000000 == 0) {
            cout << g << "," << numfit << "," << numperfect << endl;
        }
    }
    cout << nrandoms << "," << numfit << "," << numperfect << endl;

    LOG ("For " << N_Genes << " genes, there were " << numfit << " fit genomes out of " << nrandoms << " of which " << numperfect << " had F=1");
    LOG ("That's " << (double)numfit / nrandoms << " and "<< (double)numperfect / nrandoms << " as proportions.");
    return 0;
}
