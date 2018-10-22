/*
 * Tests the string format to genome conversion
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

int main (int argc, char** argv)
{
    // Initialise masks
    masks_init();

    // Try to generate the genome from Fig 1 from its string representation:
    string stringform = "01101100000001011111100010100010000100001111111000111100000001000100101011111011";
    array<genosect_t, N_Genes> shouldbe = selected_genome1();
    array<genosect_t, N_Genes> g = str2genome (stringform);


    LOG ("SHOULD BE: ");
    show_genome (shouldbe);

    LOG ("IS:        ");
    show_genome (g);

    int rtn = 0;

    for (unsigned int i = 0; i < N_Genes; ++i) {
        if (shouldbe[i] != g[i]) {
            rtn = 1;
        }
    }

    return rtn;
}
