/*
 * Tests the hamming distance code - the genome one.
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

    array<genosect_t, N_Genes> g1 = selected_genome1();
    array<genosect_t, N_Genes> g2 = selected_genome1();
    g2[0] += 1;
    g2[3] += 1;

    unsigned int h1 = compute_hamming (g1, g2);

    cout << "Hamming dist: " << h1 << endl;

    int rtn = 1;
    if (h1 == 0x2) {
        rtn = 0;
    }
    return rtn;
}
