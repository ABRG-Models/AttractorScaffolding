/*
 * Mutate a known-good genome and evaluate the fitness at various
 * Hamming distances away from the fitness = 1 original.
 *
 * This defaults to N_Genes=5; other values of N_Genes may be set.
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

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// A cutoff for the number of possible mutated Hamming states at which
// we start doing sampled searches.
#define MAX_EXHAUSTIVE 4000000

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
    // Set up the Mersenne Twister RNG
    rngDataInit (&rd);
    zigset (&rd, DUMMYARG);
    rd.seed = seed;

    // Initialise masks
    masks_init();

    // Set probability of flipping. 0.1 is good as it will ensure genomes evolve quickly.
    pOn = 0.1;

    // Holds the genome. Write to do this 10 times on different F=1
    // genomes. In practice, I do this by running this program 10
    // times in parallel.
    array<genosect_t, N_Genes> genome = evolve_new_genome();

    double f = evaluate_fitness (genome);
    show_genome (genome);
    LOG ("The fitness of the newly evolved genome is " << f);

    // Open the file.
    ofstream fout;
    stringstream fpathss;
    if (!mid.empty()) {
        fpathss << "data/mutations_" << FF_NAME << "_n" << N_Genes << "_" << mid << ".csv";
    } else {
        fpathss << "data/mutations_" << FF_NAME "_n" << N_Genes << "_" << genome_id(genome) << ".csv";
    }
    fout.open (fpathss.str().c_str());

    // FitPerState==FitnessPerFitState. TotalFit/SampledStates is the right statistic.
    // HammingDist:
    // NumFit:
    // SampledStates:
    // ProportionFit:
    // ExhaustiveSearch:
    // TotalFit:
    // FitPerState:
    fout << "HammingDist,NumFit,SampledStates,ProportionFit,ExhaustiveSearch,TotalFit,FitPerState"
         << endl;

    unsigned int nsamples = 0;
    unsigned int exhaustive_search = 0;
    // The ulong and Xint types are typedeffed and used in lmp.h and friends
    ulong l_genome = N_Genes * (1<<N_Ins);
    Xint nchoosek;
    Xint max_exhaustive; // max number for which we'll do an exhaustive search
    lmp::InitSetUi(nchoosek, 1);
    lmp::InitSetUi(max_exhaustive, MAX_EXHAUSTIVE);

    for (ulong h = 1; h <= (l_genome/3); ++h) {

        // Compute l_genome choose h to find out whether to do exhaustive
        // computation or sampled estimate of the number of fit
        // mutations for the given Hamming distance.
        lmp::BinomialUiUi (nchoosek, l_genome, h);

        cout << "Counting fit states Hamming number " << h << " away from fit genome..." << endl;

        // Now find out if we can cast nchoosek into a smaller number
        // and use it.
        double numHammingStates = 0.0;
        pair<unsigned int, double> fitmuts;
        if (lmp::Cmp(nchoosek, max_exhaustive) > 0) {

            nsamples = (MAX_EXHAUSTIVE>>2);
            numHammingStates = (double)nsamples;
            DBG ("Sampled search, choose " << nsamples << " out of " << nchoosek << " Hamming " << h << " states");
            // Work out a suitable number of samples, up to a max of MAX_EXHAUSTIVE
            exhaustive_search = 0;
            fitmuts = num_fit_mutations_sample (genome, h, nsamples);
        } else {
            // It might seem bonkers to turn nchoosek into a string then
            // back to double, but it worked, and was simpler than
            // using mpz_export() and as it's only called a few times,
            // timing isn't an issue.
            stringstream ss;
            ss << nchoosek;
            ss >> numHammingStates;
            DBG ("Exhaustive search of "  << numHammingStates << " Hamming " << h << " states");
            exhaustive_search = 1;
            fitmuts = num_fit_mutations (genome, h);
        }

        fout << h << "," << fitmuts.first << "," << numHammingStates << ","
             << (double)fitmuts.first/numHammingStates << "," << exhaustive_search << ","
             << fitmuts.second << "," << (double)fitmuts.second/numHammingStates << endl;
    }

    // Clean up the Xints
    lmp::Clear (nchoosek);
    lmp::Clear (max_exhaustive);

    // Close the file.
    fout.close();

    return 0;
}
