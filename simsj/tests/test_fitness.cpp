/*
 * Check that the functions to count fitness for exhaustive and
 * sampled searches actually work and produce the same answer
 *
 * This defaults to N_Genes=4; other values of N_Genes may be set.
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
#include <climits>
#include <lmp.h> // For factorials

using namespace std;

// Choose debugging level.
//
#define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state can be set at compile time.
#ifndef N_Genes
#define N_Genes 4
#endif

// Common code
#include "lib.h"

// The fitness function used here
#include "fitness.h"
#include "mutation.h"

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

    // Set probability of flipping. 0.1 is good as it will ensure genomes evolve quickly.
    pOn = 0.1;

    // Holds the genome. Write to do this 10 times on different F=1 genomes.
    array<genosect_t, N_Genes> genome = evolve_new_genome();

    float f = evaluate_fitness (genome);
    show_genome (genome);
    LOG ("The fitness of the newly evolved genome is " << f);

    unsigned int nsamples = 0;
    // The ulong and Xint types are typedeffed and used in lmp.h and friends
    ulong l_genome = N_Genes * (1<<N_Ins);
    Xint nchoosek;
    lmp::InitSetUi(nchoosek, 1);

    // Choose (low) test Hamming number here.
    ulong h = 3;

    // Compute l_genome choose h to find out whether to do exhaustive
    // computation or sampled estimate of the number of fit
    // mutations for the given Hamming distance.
    lmp::BinomialUiUi (nchoosek, l_genome, h);

    cout << "Counting fit states Hamming number " << h << " away from fit genome..." << endl;

    ulong numHammingStates;
    stringstream ss;
    ss << nchoosek;
    ss >> numHammingStates;
    DBG ("Exhaustive search of "  << numHammingStates << " Hamming " << h << " states");

    pair<unsigned int, double> fitmuts_ex;
    fitmuts_ex = num_fit_mutations (genome, h);

    nsamples = numHammingStates;
    DBG ("Sampled search of "  << numHammingStates << " Hamming " << h << " states");
    pair<unsigned int, double> fitmuts_samp;
    fitmuts_samp = num_fit_mutations_sample (genome, h, nsamples);

    // Clean up the Xints
    lmp::Clear (nchoosek);

    int rtn = 1;
    if (fitmuts_ex.first == fitmuts_samp.first && fitmuts_ex.second == fitmuts_samp.second) {
        rtn = 0;
    }
    return rtn;
}
