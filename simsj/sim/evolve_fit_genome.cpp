/*
 * Evolve from a random genome into a fit genome once only.
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
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// Choose debugging level.
//
//#define DEBUGF 1
//#define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#define N_Genes 5

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

    // You can set alternative target states:
    //target_ant = 0x1b; // 11011
    //target_pos = 0x11; // 10001

    // probability of flipping
    pOn = 0.2;

    if (argc > 1) {
        stringstream poss;
        poss << argv[1];
        poss >> pOn;
    }

    LOG ("pOn: " << pOn);

    // Initialise masks
    masks_init();

    // Holds the genome. Write to do this 10 times on different F=1 genomes.
    array<genosect_t, N_Genes> genome = evolve_new_genome();
    show_genome (genome);
    double f = evaluate_fitness (genome);
    LOG ("The fitness of the newly evolved genome is " << f << " (should be 1)");
    LOG ("1s and 0s representation: " << genome2str (genome));
    return 0;
}
