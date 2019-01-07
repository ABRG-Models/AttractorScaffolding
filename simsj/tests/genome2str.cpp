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

    string shouldbe = "01101100000001011111100010100010000100001111111000111100000001000100101011111011";

    LOG ("SHOULD BE: " << shouldbe);
    LOG ("IS:        " << stringis);

    int rtn = 1;
    if (stringis.compare(shouldbe) == 0) {
        rtn = 0;
    }
    return rtn;
}
