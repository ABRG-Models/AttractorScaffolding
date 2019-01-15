/*
 * Takes a genome and a fitness as args, and tests that our fitness
 * algorithm returns the same fitness.
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
    // Set the global target states:
    target_ant = 0x15; // 10101 or 21 dec
    target_pos = 0xa;  // 01010 or 10 dec

    // Seed the RNG
    srand(4);

    //probability of flipping
    pOn = 0.1;

    // Initialise masks
    masks_init();

    if (argc < 3) {
        LOG ("Usage: " << argv[0] << " 0110100101.....  0.7");
        return 1;
    }

    string s(argv[1]);
    // Expected fitness
    float ef = 0.0f;
    stringstream efss;
    efss << argv[2];
    efss >> ef;

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

    LOG ("Fitness for that genome is " << f);

    int rtn = 1;
    if (f == ef) {
        rtn = 0;
    }
    return rtn;
}
