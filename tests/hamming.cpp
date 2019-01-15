/*
 * Tests the hamming distance code
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

    state_t state_a = 0x10; // 10000
    state_t state_b = 0x11; // 10001 hamming dist 1 away from state_a

    state_t h1 = compute_hamming (state_a, state_b);

    state_b = 0xc; // 01100 vs 10000 should have hamming dist 3.
    state_t h2 = compute_hamming (state_a, state_b);

    int rtn = 1;
    if (h1 == 0x1 && h2 == 0x3) {
        rtn = 0;
    }
    return rtn;
}
