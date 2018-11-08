/*
 * Evolves genome repeatedly according to the fitness function
 * described in the paper associated with this code.
 *
 * Author: S James
 * Date: October 2018.
 */

#include <array>
#include <vector>
#include <algorithm>

#define DEBUG 1

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// Common code
#include "lib.h"

//#define DEBUGF 1
// We're analysing the basins of attraction
#include "basins.h"

using namespace std;

int main (int argc, char** argv)
{
    // Initialise masks
    masks_init();

    array<genosect_t, N_Genes> genome = {{ 0x79b2, 0xadd, 0x4382, 0x5032, 0x25d5 }};

    AllBasins ab1 (genome);
    cout << "All transitions is a set of size " << ab1.transitions.size() << endl;
    cout << "Number of basins: " << ab1.getNumBasins() << endl;
    cout << "Size of attractors: ";
    vector<unsigned int>::const_iterator i = ab1.attractorSizes.begin();
    bool first = true;
    while (i != ab1.attractorSizes.end()) {
        if (!first) { cout << ","; } else { first = false; }
        cout << *i;
        ++i;
    }
    cout << endl;

    // Evolve the genome...
    pOn = 0.02;
    evolve_genome (genome);

    AllBasins ab2 (genome);
    cout << "All transitions is a set of size " << ab2.transitions.size() << endl;
    cout << "Number of basins: " << ab2.getNumBasins() << endl;
    cout << "Size of attractors: ";
    i = ab2.attractorSizes.begin();
    first = true;
    while (i != ab2.attractorSizes.end()) {
        if (!first) { cout << ","; } else { first = false; }
        cout << *i;
        ++i;
    }
    cout << endl;

    // Comparison of transition sets
    set<unsigned int> intersection;
    set_intersection (ab1.transitions.begin(), ab1.transitions.end(),
                      ab2.transitions.begin(), ab2.transitions.end(),
                      inserter(intersection, intersection.begin()));
    cout << "Intersection between sets: " << endl;
    for (unsigned int n : intersection) {
        cout << "0x" << hex << n << dec << ' ';
    }
    cout << endl;

    set<unsigned int> difference;
    set_difference (ab1.transitions.begin(), ab1.transitions.end(),
                    ab2.transitions.begin(), ab2.transitions.end(),
                    inserter(difference, difference.begin()));
    cout << "Difference between sets: " << endl;
    for (unsigned int n : difference) {
        cout << "0x" << hex << n << dec << ' ';
    }
    cout << endl;

    return 0;
}
