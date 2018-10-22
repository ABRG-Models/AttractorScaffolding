/*
 *
 * Drift with low p value (actually whatever is provided by the user
 * on the command line)
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
#define N_Genes 5

// The number of generations to evolve for. 20000000 takes roughly 30
// seconds on an Intel Core i9-8950HK CPU.
#define N_Generations 100000000

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
        LOG ("Usage: " << argv[0] << " pOn");
        LOG ("Supply the probability of flipping a gene during drift, pOn (float, 0 to 1.0f)");
        return 1;
    }
    pOn = static_cast<float>(atof (argv[1]));
    LOG ("Probability of flipping = " << pOn);

    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);

    // Initialise masks
    masks_init();

    // generations records the relative number of generations required
    // to achieve fitness 1.
    vector<unsigned int> generations;

#ifdef RECORD_ALL_FITNESS
    // Records the evolution of the fitness of a genome. Fig 3. The
    // (abs) generation for each fitness is recorded along with
    // the floating point fitness value.
    vector<pair<unsigned int, float> > fitness;
#endif

    // Set to true when a new, random genome should be generated
    bool need_new_genome = true;

    // Holds the drifting genome
    array<genosect_t, N_Genes> genome;

    unsigned int lastgen = 0;

    // The main loop. Repeatedly evolve from a random genome starting
    // point, recording the number of generations required to achieve a
    // maximally fit state of 1.
    for (unsigned int gen = 0; gen < N_Generations; ++gen) {

        // At the start of the loop, and every time fitness of 1.0 is
        // achieved, generate a random genome starting point.
        if (need_new_genome == true) {
            DBG ("Generate new random genome");
            random_genome (genome);
            need_new_genome = false;
#ifdef RECORD_ALL_FITNESS
            fitness.push_back (make_pair(gen, 0.0f));
#endif
        }

        float f = evaluate_fitness (genome);
        DBG2 ("Fitness f = " << f);

        // Test fitness to determine whether we should evolve.
        if (f == 1.0f) {
            DBG ("Fitness max. F=" << f);
            // Record data
            generations.push_back (gen-lastgen);
            lastgen = gen;

#ifdef RECORD_ALL_FITNESS
            fitness.push_back (make_pair(gen, f));
#endif

            // Reset loop variables
            need_new_genome = true;

        } else {
            evolve_genome (genome);
        }
    }

    LOG ("Generations size: " << generations.size());

    // Save data to file. These data files can be graphed using the python scripts.
    ofstream f;
    stringstream pathss;
    pathss << "./data/drift_" << FF_NAME << "_" << N_Generations << "_gens_" << pOn << ".csv";
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
    pathss2 << "./data/drift_" << FF_NAME << "_" << N_Generations << "_fitness_" << pOn << ".csv";
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
