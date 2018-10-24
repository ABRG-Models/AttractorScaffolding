/*
 * Evaluates the starting state using the specific genome used to
 * create Fig 1. in the associated paper.
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

using namespace std;

// Choose debugging level.
//
#define DEBUGF 1
// #define DEBUG 1
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
    srand(4);

    // Initialise masks
    masks_init();

    // Holds the genome
    array<genosect_t, N_Genes> genome = selected_genome();
    float f = evaluate_fitness (genome);
    show_genome (genome);
    LOG ("Alternative format for genome:");
    cout << genome2str (genome) << endl;
    LOG ("The fitness of the selected genome is " << f);

    return 0;
}
