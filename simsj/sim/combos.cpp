/*
 * Show all of a set of combinations of size k drawn from a set size
 * n.
 */
#include <iostream>

#define N_Genes 5 // required for lib.h but unused
#include "lib.h"

using namespace std;

int main (int argc, char** argv)
{
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " ngenes limitlen" << endl;
        return 1;
    }

    int ngenes = atoi(argv[1]);
    int n = (1<<ngenes); // The size of the set; for {0, 1, 2, 3} it's 4
    int k = atoi(argv[2]); // The size of the subsets; for {0, 1}, {0, 3}, etc... it's 2

    int comb[n]; // comb[i] is the index of the i-th element in the combination

    // Initialise comb to zero
    for (int i = 0; i < n; ++i) {
        comb[i] = 0;
    }

    // Setup comb for the initial combination
    for (int i = 0; i < k; ++i) {
        comb[i] = i;
    }

    // Print the first combination
    int combo_count = 0;
    cout << "c";
    cout.fill('0');
    cout.width(2);
    cout << combo_count++ <<  endl;
    printc_binary (comb, k, ngenes);
    printc (comb, k);

    // Generate and print all the other combinations
    while (next_combination(comb, k, n)) {
        cout << "c";
        cout.fill('0');
        cout.width(2);
        cout << combo_count++ <<  endl;
        printc_binary (comb, k, ngenes);
        printc (comb, k);
        cout << "---\n";
    }

    return 0;
}
