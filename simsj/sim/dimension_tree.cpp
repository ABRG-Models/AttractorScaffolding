/*
 * Compute, by brute force, the tree of nodes and edges for an n
 * dimensional boolean space. Start at the node [000...0000] and
 * proceed with incrementing Hamming distance from the start. Make
 * sets containing the layer nodes and the transitions.
 *
 * Save out in a form suitable for graphing in python.
 */

#include <iostream>
#include <sstream>
#include <fstream>

#define N_Genes 5 // required for lib.h but unused
#include "lib.h"

#include <vector>
#include <utility>
#include <set>

using namespace std;

// Can go to 32 bits without messing about with very long numbers -
// this is because while the nodes can be 32 bit, the transition type
// needs to be 64 bits (though I could store transitions as pairs of
// node_t). Won't need anything like this many; we'd have 10^8 nodes
// in the middle layer for a n=32 dimension space, so this can be an
// unsigned int.
typedef unsigned int node_t;

// A privately defined compute hamming function
node_t
_compute_hamming (node_t n, node_t state, node_t target)
{
#if 0
    cout << "Comparing " << state << " and " << target << " for hamming dist." <<endl;
    node_t hamming = n;
    if (state != target) {
        for (unsigned int i = 0; i < n; ++i) {
            // Bit i in state
            if (((state & (0x1 << i))>>i) == ((target & (0x1 << i))>>i)) {
                hamming--;
            }
        }
    } else {
        hamming = 0;
    }
    return hamming;
#endif
    node_t bits = state ^ target;
    unsigned int hamming = _mm_popcnt_u32 ((unsigned int)bits);
    return static_cast<node_t>(hamming);
}

int main (int argc, char** argv)
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " n" << endl;
        cerr << "Build an n-dimensional hyperpolygon graph." << endl;
        return 1;
    }

    node_t n = atoi(argv[1]);
    if (n >= 16) { // Could be increased to 32 but results will be fairly useless.
        cerr << "Please choose n <= 16 (probably 6, 7 or 8 is a sensible maximum)" << endl;
        return 1;
    }
    node_t n_states = 1<<n;
    int set_bits = 0;

    vector<set<node_t> > node_layers;
    vector<set<pair<node_t, node_t> > > transition_layers;

    for (node_t n_ones = 0; n_ones <= n; ++n_ones) {
        set<node_t> nodes;
        set<pair<node_t, node_t> > trans;
        for (node_t i = 0; i < n_states; ++i) {
            set_bits = _mm_popcnt_u32 ((unsigned int)i);
            if (set_bits == n_ones) {
                nodes.insert(i);

                // If not on the first layer, determine which of the
                // members of the previous layer set can connect to this
                // state with a single bit flip.
                if (n_ones > 0) {
                    set<node_t>::const_iterator j = node_layers.back().begin();
                    while (j != node_layers.back().end()) {
                        node_t hd = _compute_hamming (n, *j, i);
                        if (hd == 1) {
                            // Hamming separation is 1. Add transition
                            trans.insert (make_pair (*j, i));
                        }
                        ++j;
                    }
                }
            }
        }
        node_layers.push_back (nodes);
        if (n_ones > 0) {
            transition_layers.push_back (trans);
        }
    }

    // Write results out
    for (node_t n_ones = 0; n_ones <= n; ++n_ones) {

        ofstream f_nodes;
        stringstream nodespath;
        nodespath << "data/node_layer_" << n_ones << "_n" << n << ".csv";
        f_nodes.open (nodespath.str().c_str(), ios::out|ios::trunc);
        if (!f_nodes.is_open()) {
            cerr << "File open error" << endl;
            return 1;
        }
        set<node_t>::const_iterator j = node_layers[n_ones].begin();
        while (j != node_layers[n_ones].end()) {
            f_nodes << *j << endl;
            ++j;
        }
        f_nodes.close();

        if (n_ones > 0) {
            ofstream f_trans;
            stringstream transpath;
            transpath << "data/node_trans_layer_" << n_ones << "_n" << n << ".csv";
            f_trans.open (transpath.str().c_str(), ios::out|ios::trunc);
            if (!f_trans.is_open()) {
                cerr << "File open error" << endl;
                return 1;
            }
            set<pair<node_t, node_t> >::const_iterator k = transition_layers[n_ones-1].begin();
            while (k != transition_layers[n_ones-1].end()) {
                f_trans << k->first << "," << k->second << endl;
                ++k;
            }
            f_trans.close();
        }
    }

    return 0;
}
