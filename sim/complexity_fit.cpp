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
#include <fstream>

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
#include "quine.h"
#include "basins.h"

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
    // Set up the Mersenne Twister RNG
    rngDataInit (&rd);
    zigset (&rd, DUMMYARG);
    rd.seed = seed;

    // Make the evolutions as fast as possible
    pOn = 0.05;

    // Initialise masks
    masks_init();

    // To store the results of this program
    map<unsigned int, unsigned int> canalvalues;
    double complexity = 0.0;

    unsigned int ntrials = 10000;

    double numBasins = 0.0;
    double meanAttractorLen = 0.0;

    array<genosect_t, N_Genes > genome;

    ofstream fout;
    fout.open ("./data/complexity_fit.csv", ios::out|ios::trunc);
    if (!fout.is_open()) {
        cerr << "Failed to open output file." << endl;
        return -1;
    }

    // Header
    fout << "canalyzingness,QMcomplexity,numBasins,meanAttractorLen,bias" << endl;

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
        fout << c << ",";

        // Determine complexity (Quine-McCluskey algorithm)
        double cmplx = 0.0;
        for (unsigned int i = 0; i < N_Genes; ++i) {
            Quine Q(N_Genes);
            for (unsigned int j = 0; j<(1<<N_Genes); ++j) {
                if ((genome[i]>>j)&0x1) {
                    Q.addMinterm(j);
                }
            }
            Q.go();
            cmplx += Q.complexity();
        }
        cmplx /= (double)N_Genes; // mean complexity per gene
        complexity += cmplx;
        fout << cmplx << ",";

        // Basins analysis
        AllBasins ab (genome);
        unsigned int nb = ab.getNumBasins();
        fout << nb << ",";
        numBasins += static_cast<double>(nb);
        double mal = ab.meanAttractorLength();
        fout << mal << ",";
        meanAttractorLen += mal;

        // Bias - set bits vs unset bits
        double b = bias (genome);
        fout << b << endl;
    }
    complexity /= (double)N_Genes;
    complexity /= (double)ntrials;
    numBasins /= (double)ntrials;
    meanAttractorLen /= (double)ntrials;

    fout.close();

    // Output results
    cout << "Number of fit genomes tested: " << ntrials << endl;
    auto m = canalvalues.begin();
    while (m != canalvalues.end()) {
        cout << "Canalyzation value " << m->first << " seen " << m->second << " times" << endl;
        ++m;
    }
    cout << "Mean complexity of fit genomes: " << complexity << endl;
    cout << "Mean number of basins of attraction: " << numBasins << endl;
    cout << "Mean attractor length: " << meanAttractorLen << endl;

    return 0;
}
