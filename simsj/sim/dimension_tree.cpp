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

//#define DEBUG 1

#define N_Genes 5 // required for lib.h but unused
#include "lib.h"

#include <vector>
#include <utility>
#include <set>
#include <map>
#include <deque>

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
    node_t bits = state ^ target;
    unsigned int hamming = _mm_popcnt_u32 ((unsigned int)bits);
    return static_cast<node_t>(hamming);
}

void find_paths (map<node_t, deque<node_t> >& paths,
                 vector<set<pair<node_t, node_t> > >& transition_layers,
                 node_t pathto_state,
                 int set_bits_pathto)
{
    DBG ("Called for transition layer " << set_bits_pathto);

    if (set_bits_pathto == -1) {
        DBG ("Returning as set_bits_pathto is -1");
        return;
    }
    // For each member of transition layers[set_bits_pathto],
    // see if its second element is equal to pathto_state. For each one that IS...
    for (pair<node_t, node_t>trans : transition_layers[set_bits_pathto]) {
        DBG2 ("Transitions to " << set_bits_pathto << " set bits layer: "  << trans.first << "->" << trans.second << "...");
        if (trans.second == pathto_state) {
            DBG ("Transition " << trans.first << "->" << trans.second << " ends on pathto_state");
            // Now check through paths to make sure we don't need to add to an existing path
            map<node_t, deque<node_t> >::iterator pi = paths.begin();
            bool added = false;
            int max_path_idx = paths.size() - 1;
            DBG ("max_path_idx = " << max_path_idx);
            while (pi != paths.end()) {
                DBG ("Checking path " << pi->first << " added is " << added << ", comparing path front: " << pi->second.front() << " with pathto_state: " << pathto_state);
                if (pi->second.front() == pathto_state) {
                    DBG ("Adding " << trans.first << " to path " << pi->first);
                    pi->second.push_front (trans.first);
                    added = true;
                }
                if (pi->first > max_path_idx) {
                    max_path_idx = pi->first;
                }

                ++pi;
            }
            if (!added) {
                DBG ("Adding new path " << trans.first << "->" << trans.second << " to paths index " << (max_path_idx+1));
                deque<node_t> new_deque;
                new_deque.push_back (trans.first);
                new_deque.push_back (trans.second);
                paths.insert (make_pair (max_path_idx+1, new_deque));
            }
            // Recurse
            DBG ("Recursion to trans layer " << (set_bits_pathto-1) << ", paths size is " << paths.size());
            find_paths (paths, transition_layers, trans.first, set_bits_pathto-1);
            DBG ("After recursion, paths size is " << paths.size());
        } // else do nothing
    }
    // At end, recurse?
    //find_paths (paths, transition_layers, trans.first, set_bits_pathto-1);
}

int main (int argc, char** argv)
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " n [target state to count paths to]" << endl;
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

    // Now find the number of unique paths to a specific state.
    if (argc > 2) {
        node_t pathto_state = atoi(argv[2]);
        if (pathto_state >= (1<<n)) {
            cerr << "pathto_state should be in range [0,"<< ((1<<n)-1) << "]" << endl;
            return 1;
        }
        int set_bits_pathto = _mm_popcnt_u32 ((unsigned int)pathto_state);
        cout << "Finding paths to the state "<< pathto_state << " which has " << set_bits_pathto << " set bits." << endl;

        if (set_bits_pathto == 0) {
            cout << "You're already there!" << endl;
            return 0;
        }

        // Recursive function required to find all the paths.
        map<node_t, deque<node_t> > paths;
        find_paths (paths, transition_layers, pathto_state, set_bits_pathto-1);
        map<node_t, deque<node_t> >::iterator pi = paths.begin();
        cout << "There are " << paths.size() << " paths from 0 to " << pathto_state << endl;
#ifdef SHOW_PATHS
        while (pi != paths.end()) {
            cout << "Path " << pi->first << ": ";
            for (node_t n : pi->second) {
                cout << n << ",";
            }
            cout << endl;
            ++pi;
        }
#endif

        // Answer:
        // 1, 1*2, 1*2*3, 1*2*3*4, etc
        //
        // There are h! ways to traverse from one vertex to another
        // vertex in a layer which is a Hamming distance h away from
        // the starting vertex.

#if 0
        // Investigating each layer.
        set<node_t> target_states;
        set<node_t> next_target_states;
        target_states.insert(pathto_state);
        while (set_bits_pathto > 0) {
            next_target_states.clear();
            // First find the target state in its layer.
            set<pair<node_t, node_t> >::const_iterator k = transition_layers[set_bits_pathto-1].begin();
            cout << "Its layer:" << endl;
            while (k != transition_layers[set_bits_pathto-1].end()) {
                for (node_t ts : target_states) {
                    if (k->second == ts) {
                        // There is a transition in this layer to a target state.
                        cout << k->first << "," << k->second << endl;
                        next_target_states.insert (k->first);
                    }
                }
                ++k;
            }
            target_states.swap (next_target_states);
            --set_bits_pathto;
        }
#endif
    }

    return 0;
}
