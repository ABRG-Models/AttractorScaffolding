/*
 * Tests the genome to string format conversion
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

    array<genosect_t, N_Genes> genome = selected_genome();

    show_genome(genome);

    string stringis = genome2str (genome);

    string shouldbe = "0101111010001010101011100001000110000111111000010111100000111010001010111001100111110111011100011111111001100010001111000101100001100011101011000100111011111011";

    LOG ("SHOULD BE: " << shouldbe);
    LOG ("IS:        " << stringis);

    int rtn = 1;
    if (stringis.compare(shouldbe) == 0) {
        rtn = 0;
    }
    return rtn;
}
