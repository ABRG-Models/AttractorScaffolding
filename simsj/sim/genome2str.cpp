/*
 * Convert from genome_id format
 * (e.g. 5039a8e4-a090a0eb-56cfd0a8-9c9ccdbb-60b214b) to 1s and 0s
 * format.
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
// #define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#define N_Genes 5

// Common code
#include "lib.h"

int main (int argc, char** argv)
{
    // Initialise masks
    masks_init();

    if (argc < 2) {
        LOG ("Usage: " << argv[0] << " 5039a9e4-abc.....");
        return 1;
    }

    string s(argv[1]);

    array<genosect_t, N_Genes> genome;
    zero_genome(genome);

    // Split s using the '-' character, then set genome with each one.
    bool ignoreTrailingEmptyVal = true;
    vector<string> vs = stringToVector (s, "-", ignoreTrailingEmptyVal);
    unsigned int g = 0;
    for (string gs : vs) {
        stringstream ss;
        ss << hex << gs;
        ss >> genome[g];
        g++;
    }

    //show_genome(genome);

    string stringis = genome2str (genome);
    cout << stringis << endl;

    return 0;
}
