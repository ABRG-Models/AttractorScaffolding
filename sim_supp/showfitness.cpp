/*
 * Taking a genome in "Dan string format", convert to array<genosect_t, N_Genes> and then evaluate
 * the fitness. Also evaluate the comlexity and canalysingness
 *
 * Author: S James
 * Date: October 2018. Complexity work, September 2019.
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
#include "quine.h"

using namespace std;

// Choose debugging level.
//
#define DEBUG 1
// #define DEBUG2 1
#define DEBUGF 1

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// Common code
#include "lib.h"

#include "fitness.h"

#include "basins.h"

int main (int argc, char** argv)
{
    // Seed the RNG
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);
    // Set up the Mersenne Twister RNG
    rngDataInit (&rd);
    zigset (&rd, DUMMYARG);
    rd.seed = seed;

    //probability of flipping
    pOn = 0.1;

    // Initialise masks
    masks_init();

    if (argc > 2) {
        LOG ("Usage: " << argv[0] << " 0110100101..... (or omit string to show a random genome)");
        return 1;
    }

    string s("");
    if (argc == 2) {
        string s1(argv[1]);
        s = s1;

        unsigned int l = s.length();
        unsigned int l_genosect = 1 << N_Ins;
        unsigned int l_genome = N_Genes * l_genosect;
        if (l == l_genome) {
            LOG ("String has " << l_genome << " bit chars as required...");
        } else {
            LOG ("String does not have " << l_genome << " bit chars as required. Exiting.");
            return 1;
        }
    } else {
        array<genosect_t, N_Genes> g1;
        random_genome(g1);
        s = genome2str (g1);
    }

    array<genosect_t, N_Genes> g = str2genome (s);

    show_genome (g);

    AllBasins ab1 (g);

    cout << "All transitions is a set of size " << ab1.transitions.size() << endl;
    cout << "Number of basins: " << ab1.getNumBasins() << endl;
    for (unsigned int ii = 0; ii<ab1.getNumBasins(); ++ii) {
        ab1.basins[ii].debug();
    }
    cout << "Size of attractors: ";
    vector<unsigned int>::const_iterator i = ab1.attractorSizes.begin();
    bool first = true;
    while (i != ab1.attractorSizes.end()) {
        if (!first) { cout << ","; } else { first = false; }
        cout << *i;
        ++i;
    }
    cout << endl;

    double f = evaluate_fitness (g);
    LOG ("Fitness (using " << FF_NAME << ") for that genome is " << f);

    double cmplx = 0.0;
    for (unsigned int i = 0; i < N_Genes; ++i) {
        Quine Q(N_Genes);
        for (unsigned int j = 0; j<(1<<N_Genes); ++j) {
            if ((g[i]>>j)&0x1) {
                Q.addMinterm(j);
            }
        }
        Q.go();
        cmplx += Q.complexity();
    }
    cout << "Mean complexity: " << (cmplx/(double)N_Genes) << "/" << (1<<N_Genes) << endl;

    return 0;
}
