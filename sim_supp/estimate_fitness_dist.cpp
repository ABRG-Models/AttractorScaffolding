/*
 * Randomly selects N_Genomes genomes and finds the fitness of
 * each. The idea is to determine, by sampling, the distribution of
 * fitnesses for the chosen fitness function.
 *
 * Author: S James
 * Date: November 2018.
 */

#include <iostream>
#include <vector>
#include <set>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
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

// The number of genomes to generate
#ifndef N_Genomes
# define N_Genomes 1000000
#endif

// Common code
#include "lib.h"

// The fitness function used here
#include "fitness.h"

// Perform a loop N_Generations long during which an initially
// randomly selected genome is evolved until a maximally fit state is
// achieved.
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

    // The fitnesses that we find, the fitness itself is the key, the
    // number of times that fitness is found is the value.
    map<double, unsigned long long int> fitnesses;

    array<genosect_t, N_Genes> genome;
    for (unsigned int i = 0; i < N_Genomes; ++i) {
        random_genome (genome);
        double f = evaluate_fitness (genome);
        fitnesses[f]++;
    }

    LOG ("fitnesses.size(): " << fitnesses.size());
    ofstream fout;
    stringstream pathss;
    pathss << "./data/fitness_dist_" << FF_NAME << "_n" << N_Genomes << ".csv";

    fout.open (pathss.str().c_str(), ios::out|ios::trunc);
    if (!fout.is_open()) {
        cerr << "Error opening " << pathss.str() << endl;
        return 1;
    }

    auto mi = fitnesses.begin();
    while (mi != fitnesses.end()) {
        fout << mi->first << "," << mi->second << endl;
        ++mi;
    }

    fout.close();

    return 0;
}
