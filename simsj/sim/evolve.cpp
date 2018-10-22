/*
 * Evolves genome repeatedly according to the fitness function
 * described in the paper associated with this code.
 *
 * Author: S James
 * Date: October 2018.
 */

#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// Choose debugging level. If you uncomment one of these, then you
// should probably reduce N_Generations
//
// #define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// The number of generations to evolve for. 20000000 takes roughly 30
// seconds on an Intel Core i9-8950HK CPU.
#define N_Generations 100000000
// How often to make a message print out about progress. Make 10 or
// 100 times smaller than N_Generations.
#define N_Genview       1000000

// Whether to create large files with all the fitness information,
// which can be used to make up graphs showing evolution of the
// fitness.
//#define RECORD_ALL_FITNESS 1

// Common code
#include "lib.h"

// The fitness function used here
#include "fitness.h"

// Perform a loop N_Generations long during which an initially
// randomly selected genome is evolved until a maximally fit state is
// achieved.
int main (int argc, char** argv)
{
    // Obtain pOn from command line.
    if (argc < 2) {
        LOG ("Usage: " << argv[0] << " pOn target_ant target_pos");
        LOG ("Supply the probability of flipping a gene during evolution, pOn (float, 0 to 1.0f)");
        LOG ("Optionally supply the anterior and posterior targets (integer, in range 0 to 31)");
        return 1;
    }
    pOn = static_cast<float>(atof (argv[1]));
    LOG ("Probability of flipping = " << pOn);

    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);

    // Initialise masks
    masks_init();

    // Alternative target states are set from the command line;
    // otherwise, the targets default to the standard opposing
    // gradeints, as initialised in lib.h.
    if (argc >= 3) {
        target_ant = static_cast<state_t>(atoi(argv[2]));
        LOG ("Anterior target: " << target_ant << " which is " << (state_str (target_ant)));
        target_pos = static_cast<state_t>(atoi(argv[3]));
        LOG ("Posterior target: " << target_pos << " which is " << (state_str (target_pos)));
    }

    // generations records the relative number of generations required
    // to achieve fitness 1.
    vector<unsigned int> generations;

#ifdef RECORD_ALL_FITNESS
    // Records the evolution of the fitness of a genome. Fig 3. The
    // (abs) generation for each fitness is recorded along with
    // the floating point fitness value.
    vector<pair<unsigned int, float> > fitness;
#endif

    // Holds the genome and a copy of it.
    array<genosect_t, N_Genes> refg;
    array<genosect_t, N_Genes> newg;

    // The main loop. Repeatedly evolve from a random genome starting
    // point, recording the number of generations required to achieve a
    // maximally fit state of 1.
    unsigned int gen = 0;
    unsigned int lastgen = 0;
    while (gen < N_Generations) {

        // At the start of the loop, and every time fitness of 1.0 is
        // achieved, generate a random genome starting point.
        random_genome (refg);
        //LOG ("New random genome:");
        //show_genome(refg);
        // Make a copy of the genome, in case evolving it leads to a
        // less fit genome, then evaluate the fitness of the genome.
        float a = evaluate_fitness (refg);
        //LOG ("New random genome generated with fitness:" << a);
        if (gen > 0) {
            generations.push_back (gen-lastgen);
            lastgen = gen;
        }
        ++gen;

        // Test fitness to determine whether we should evolve.
        while (a < 1.0f) {
            copy_genome (refg, newg);
            evolve_genome (newg);
            ++gen;

            if (gen > 0 && (gen % N_Genview == 0)) {
                LOG ("[pOn=" << pOn << "] That's " << gen/1000000.0 << "M generations (out of "
                     << N_Generations/1000000.0 << "M) done...");
            }

            if (gen >= N_Generations) {
                break;

            }
            float b = evaluate_fitness (newg);
            if (a > b) {
                // New fitness <= old fitness
                // Record _existing_ fitness f, not new fitness.
#ifdef RECORD_ALL_FITNESS
                fitness.push_back (make_pair(gen, a));
#endif
            } else {
#ifdef RECORD_ALL_FITNESS
                // Record new fitness
                fitness.push_back (make_pair(gen, b));
#endif
                // Copy new fitness to ref
                a = b;
                // Copy new to reference
                copy_genome (newg, refg);
            }
        }
    }

    LOG ("Generations size: " << generations.size());

    // Save data to file.
    ofstream f;
    stringstream pathss;
    pathss << "./data/evolve_";
    pathss << "a" << target_ant << "_p" << target_pos << "_";
    pathss << FF_NAME << "_" << N_Generations << "_gens_" << pOn << ".csv";

    f.open (pathss.str().c_str());
    if (!f.is_open()) {
        cerr << "Error opening " << pathss.str() << endl;
        return 1;
    }
    for (unsigned int i = 0; i < generations.size(); ++i) {
        f << generations[i] << endl;
    }
    f.close();

#ifdef RECORD_ALL_FITNESS
    stringstream pathss2;
    pathss2 << "./data/evolve_";
    pathss2 << "a" << target_ant << "_p" << target_pos << "_";
    pathss2 << FF_NAME << "_" << N_Generations <<  "_fitness_" << pOn << ".csv";
    f.open (pathss2.str().c_str());
    if (!f.is_open()) {
        cerr << "Error opening " << pathss2.str() << endl;
        return 1;
    }
    for (unsigned int i = 0; i < fitness.size(); ++i) {
        f << fitness[i].first << "," << fitness[i].second << endl;
    }
    f.close();
#endif

    return 0;
}
