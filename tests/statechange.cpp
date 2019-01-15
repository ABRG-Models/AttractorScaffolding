/*
 * Tests the state change algorithm works.
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

// The fitness function used here
#include "fitness.h"

int main (int argc, char** argv)
{
    // Initialise masks
    masks_init();

    state_t initial_state = 0x10; // 10000
    state_t state = initial_state;

    array<genosect_t, N_Genes> genome = selected_genome();
    compute_next (genome, state);

    show_genome (genome);

    cout << "Initial state:" << endl;
    show_state (initial_state);
    cout << "Final state:" << endl;
    show_state (state);

    cout << "Initial state: 0x" << hex << (unsigned int)initial_state
         << "; New state: 0x" << (unsigned int)state << dec << endl;

    int rtn = 1;
    if (state == 0x17) {
        rtn = 0;
    }
    return rtn;
}
