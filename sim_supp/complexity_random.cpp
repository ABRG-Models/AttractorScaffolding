/*
 * Analyse the complexity, including the number of attractor basins for randomly
 * selected genomes.
 *
 * This defaults the N_Genes=5, but N_Genes can be defined on the compiler command
 * line to find the results for other values.
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
#include "quine.h"
#include "basins.h"

#include <climits>

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

    // Unused, but set, in this program.
    pOn = 0.5;

    // To store the results of this program
    map<unsigned int, unsigned int> canalvalues;
    double complexity = 0.0;

    unsigned int ntrials = 10000;

    double numBasins = 0.0;
    double meanAttractorLen = 0.0;

    ofstream fout;
    fout.open ("./data/complexity_random.csv", ios::out|ios::trunc);
    if (!fout.is_open()) {
        cerr << "Failed to open output file." << endl;
        return -1;
    }

    array<genosect_t, N_Genes > genome;
    for (unsigned int i = 0; i < ntrials; ++i) {
        random_genome (genome);

        // Determine canalysingness
        unsigned int c = canalyzingness(genome);
        if (c > 0) {
            cout << "." << flush;
        }
        if ((i % 100000) == 0) {
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
        fout << mal << endl;
        meanAttractorLen += mal;
    }
    complexity /= (double)N_Genes;
    complexity /= (double)ntrials;
    numBasins /= (double)ntrials;
    meanAttractorLen /= (double)ntrials;

    fout.close();

    // Output results
    cout << "Number of genomes tested: " << ntrials << endl;
    auto m = canalvalues.begin();
    while (m != canalvalues.end()) {
        cout << "Canalyzation value " << m->first << " seen " << m->second << " times" << endl;
        ++m;
    }
    cout << "Mean complexity: " << complexity << endl;
    cout << "Mean number of basins of attraction: " << numBasins << endl;
    cout << "Mean attractor length: " << meanAttractorLen << endl;

    return 0;
}
