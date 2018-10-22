/*
 * Taking a genome in "Dan string format", convert to
 * array<genosect_t, N_Genes> and then evaluate the fitness.
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
#define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#define N_Genes 5

// Common code
#include "lib.h"

#include "fitness.h"

int main (int argc, char** argv)
{
    // Seed the RNG
    srand(4);

    //probability of flipping
    pOn = 0.1;

    // Initialise masks
    masks_init();

    if (argc < 2) {
        LOG ("Usage: " << argv[0] << " 0110100101.....");
        return 1;
    }

    string s(argv[1]);

    unsigned int l = s.length();
    unsigned int l_genosect = 1 << N_Ins;
    unsigned int l_genome = N_Genes * l_genosect;
    if (l == l_genome) {
        LOG ("String has " << l_genome << " bit chars as required...");
    } else {
        LOG ("String does not have " << l_genome << " bit chars as required. Exiting.");
        return 1;
    }


    array<genosect_t, N_Genes> g = str2genome (s);

    show_genome (g);

    float f = evaluate_fitness (g);

    LOG ("Fitness (using " << FF_NAME << ") for that genome is " << f);

    return 0;
}
