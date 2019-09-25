/*
 * Generate 10 non-zero fit genomes and show them, along with their
 * fitness.
 *
 * Author: S James
 * Date: November 2018.
 */

#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
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

// The number of starting genomes to generate
#ifndef N_Starts
# define N_Starts 10
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

    // Holds the starting genomes
    array<array<genosect_t, N_Genes>, N_Starts> genomes;
    // Store the fitnesses of all the starting genomes
    array<double, N_Starts> initialFitness;

    // Explicitly init to 0
    for (unsigned int g = 0; g < N_Starts; ++g) {
        initialFitness[g] = 0.0;
    }

    // Initialise genomes
    for (unsigned int g = 0; g < N_Starts; ++g) {
        while (initialFitness[g] == 0.0) {
            random_genome (genomes[g]);
            initialFitness[g] = evaluate_fitness (genomes[g]);
        }
    }

    // Output fitness
    cout.precision(12);
    for (unsigned int g = 0; g < N_Starts; ++g) {
        cout << genome2str(genomes[g]) << " has fitness " << initialFitness[g]
#if defined SHOW_ID_ALSO
             << " (" << genome_id(genomes[g]) << ")"
#endif
             << endl;
    }

    return 0;
}
