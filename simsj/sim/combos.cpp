/*
 * Show all of a set of combinations of size k drawn from a set size
 * n.
 */
#include <iostream>
#include <map>

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
    int l = atoi(argv[2]); // The size of the subsets; for {0, 1}, {0, 3}, etc... it's 2

    int zmask = 0;
    if (argc > 3) {
        zmask = atoi(argv[3]);
    }

    int comb[n]; // comb[i] is the index of the i-th element in the combination

    // Initialise comb to zero
    for (int i = 0; i < n; ++i) {
        comb[i] = 0;
    }

    // Setup comb for the initial combination
    for (int i = 0; i < l; ++i) {
        comb[i] = i;
    }

    // Print the first combination
    int combo_count = 0;
    cout << "c";
    cout.fill('0');
    cout.width(2);
    cout << combo_count++ <<  endl;

    map<float, unsigned int> scores;
    float score = 0.0f;
    int zs = 0;
    zs += printc_binary (comb, l, ngenes, zmask, score);
    scores[score]++;
    printc (comb, l);

    // Generate and print all the other combinations
    while (next_combination(comb, l, n)) {
        cout << "c";
        cout.fill('0');
        cout.width(2);
        cout << combo_count++ <<  endl;
        zs += printc_binary (comb, l, ngenes, zmask, score);
        scores[score] = scores[score] + 1;
        printc (comb, l);
        cout << "---\n";
    }

    if (zmask) {
        cout << "Number of zs is " << zs << endl;
        cout << "zmask was " << zmask << endl;
    }

    auto mi = scores.begin();
    while (mi != scores.end()) {
        cout << mi->first << "," << mi->second << endl;
        ++mi;
    }
    cout << "Total combinations: " << combo_count << endl;
    cout << "P(0) = " << (scores[0.0f]/(float)combo_count) << endl;
    cout << "P(!0) = " << (1.0f - scores[0.0f]/(float)combo_count) << endl;

    return 0;
}
