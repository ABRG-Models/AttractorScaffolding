/*
 * Evolve a genome once. See how many bits flip.
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
#define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#define N_Genes 5

// Common code
#include "lib.h"

// The fitness function used here
#include "fitness.h"

int main (int argc, char** argv)
{
    // Set the global target states:
    target_ant = 0x15; // 10101 or 21 dec
    target_pos = 0xa;  // 01010 or 10 dec

    // Seed the RNG
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);

    //probability of flipping
    if (argc > 1) {
        pOn = atof (argv[1]);
    } else {
        pOn = 0.5;
    }

    if (argc > 1) {
        stringstream poss;
        poss << argv[1];
        poss >> pOn;
    }

    LOG ("pOn: " << pOn);

    // Initialise masks
    masks_init();

    // Holds the genome
    array<genosect_t, N_Genes> genome = selected_genome();
    double f = evaluate_fitness (genome);
    LOG ("BEFORE The fitness of the selected genome is " << f);
    show_genome (genome);

    evolve_genome (genome);
    show_genome (genome);
    f = evaluate_fitness (genome);
    LOG ("AFTER The fitness of the selected genome is " << f);
    LOG ("pOn: " << pOn);
    return 0;
}
