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
#ifndef N_Generations
# define N_Generations   100000000
#endif
// How often to make a message print out about progress. Make 10 or
// 100 times smaller than N_Generations.
#define N_Genview       1000000

// Whether to create large files with all the fitness information,
// which can be used to make up graphs showing evolution of the
// fitness.
//#define RECORD_ALL_FITNESS 1

// Common code
#include "lib.h"

#ifdef RECORD_ALL_FITNESS
# include "basins.h"
# include <algorithm>
#endif

// The fitness function used here
#include "fitness.h"

struct geninfo {
    geninfo (unsigned long long int _gen, unsigned long long int _gen_0, double _fit)
        : gen(_gen)
        , gen_0(_gen_0)
        , fit(_fit)
        {}
    unsigned long long int gen;   // generations since last increase in fitness
    unsigned long long int gen_0; // generation since last F=1
    double fit;                   // The fitness
};

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
    if (argc >= 4) {
        target_ant = static_cast<state_t>(atoi(argv[2]));
        LOG ("Anterior target: " << target_ant << " which is " << (state_str (target_ant)));
        target_pos = static_cast<state_t>(atoi(argv[3]));
        LOG ("Posterior target: " << target_pos << " which is " << (state_str (target_pos)));
    }

    // generations records the relative generation number, and the
    // fitness. Every entry in this records an increase in the fitness
    // of the genome.
    vector<geninfo> generations;

#ifdef RECORD_ALL_FITNESS
    // Records the evolution of the fitness of a genome. Fig 3. The
    // (abs) generation for each fitness is recorded along with the
    // floating point fitness value. Record this in a vector of
    // vectors, with one vector for each evolution towards F=1
    vector<vector<NetInfo> > netinfo;
    vector<NetInfo> ni0;
    netinfo.push_back (ni0);
#endif

    // Holds the genome and a copy of it.
    array<genosect_t, N_Genes> refg;
    array<genosect_t, N_Genes> newg;

    // The main loop. Repeatedly evolve from a random genome starting
    // point, recording the number of generations required to achieve a
    // maximally fit state of 1.
    unsigned long long int gen = 0;
    unsigned long long int lastgen = 0;
    unsigned long long int lastf1 = 0;

    while (gen < N_Generations) {

        // At the start of the loop, and every time fitness of 1.0 is
        // achieved, generate a random genome starting point.
        random_genome (refg);

        // Make a copy of the genome, in case evolving it leads to a
        // less fit genome, then evaluate the fitness of the genome.
        double a = evaluate_fitness (refg);

#ifdef RECORD_ALL_FITNESS
        AllBasins ab_a (refg);
#endif
        //LOG ("New random genome generated with fitness:" << a);
        ++gen; // Because we randomly generated.

        // Note that if a==1.0 after the call to random_genome(), we
        // should cycle around and call random_genome() again.

        // Test fitness to determine whether we should evolve.
        while (a < 1.0) {
            copy_genome (refg, newg);
#ifdef RECORD_ALL_FITNESS
            AllBasins ab1 (newg);
#endif
            evolve_genome (newg);
#ifdef RECORD_ALL_FITNESS
            AllBasins ab2 (newg);
            set<unsigned int> diffs;
            set_difference (ab1.transitions.begin(), ab1.transitions.end(),
                            ab2.transitions.begin(), ab2.transitions.end(),
                            inserter(diffs, diffs.begin()));
#endif
            ++gen; // Because we evolved

            if (gen > 0 && (gen % N_Genview == 0)) {
                LOG ("[pOn=" << pOn << "] That's " << gen/1000000.0 << "M generations (out of "
                     << N_Generations/1000000.0 << "M) done...");
            }

            if (gen >= N_Generations) {
                break;
            }
            double b = evaluate_fitness (newg);

// REQUIRED to speed up evolution! In this case, the reference genome
// is updated when the new genome is only equally fit to the old
// genome. This means that the evolve_genome is regularly evolving
// from new, alternative genome starting points. You can imagine that
// if one genome is evolved 1000 times, and at no time provides an
// increase in fitness, but on the 1001 time is provides an equal
// fitness, it may be that the new, equally fit genome could have a
// higher probability of evolving to a fitter genome than the older
// equally fit genome. Thus, drift allows "difficult to evolve fitter"
// genomes to be discarded. This suggests that the "evolvability" of
// some genomes is higher than for others.
#ifdef STIPULATE_DRIFT_CASE // Set in CMakeLists.txt
            if (b < a) { // DRIFT. New fitness < old fitness
#else
            if (b <= a) { // NO DRIFT. New fitness <= old fitness
#endif
                // Record _existing_ fitness f, not new fitness.
#ifdef RECORD_ALL_FITNESS
                NetInfo ni(ab_a, gen, a);
                ni.deltaF = 0.0;
                netinfo.back().push_back (ni);
#endif
                // ...but otherwise, do nothing.
            } else {
#ifdef RECORD_ALL_FITNESS
                DBG2 ("New fitness is greater than old fitness! Fitness:" << b << " differences since refg: " << diffs.size());
                // Record new fitness, even if a==b - this is the "drift case".
                // Also examing the new basins of attraction and then compare ab_a and ab_b.
                AllBasins ab_b (newg);
                // Find the difference in the transitions defined by
                // the two different genomes
                set<unsigned int> difference;
                set_difference (ab_a.transitions.begin(), ab_a.transitions.end(),
                                ab_b.transitions.begin(), ab_b.transitions.end(),
                                inserter(difference, difference.begin()));
                NetInfo ni (ab_b, gen, b);
                ni.numChangedTransitions = difference.size();
                ni.deltaF = static_cast<double>(b - a);
                netinfo.back().push_back (ni);
#endif
                // Record the fitness increase in generations:
                generations.push_back (geninfo(gen-lastgen, gen-lastf1, b));
                lastgen = gen;
                if (b==1.0) {
                    lastf1 = gen;
                }

                // Copy new fitness to ref
                a = b;
                // Copy new to reference
                copy_genome (newg, refg);
#ifdef RECORD_ALL_FITNESS
                ab_a.update (refg);
#endif
            }
        }

#ifdef RECORD_ALL_FITNESS
        netinfo.back().push_back (NetInfo(ab_a, gen, a));
        if (gen < N_Generations) {
            vector<NetInfo> vni;
            netinfo.push_back (vni);
        }
        // Plus also analyse the basins of attraction and save this
        // information. The most compact way to save this information
        // is simply to save the genome.
#endif
    }

    LOG ("Generations size: " << generations.size());

    // Save data to file.
    ofstream f, f1;
    stringstream pathss;
    pathss << "./data/";
#ifdef RECORD_ALL_FITNESS
    pathss << "evolutions/";
#endif
#ifdef STIPULATE_DRIFT_CASE
    pathss << "evolve_";
#else
    pathss << "evolve_nodrift_";
#endif
#ifdef RECORD_ALL_FITNESS
    pathss << "withf_";
#endif
    pathss << "a" << (unsigned int)target_ant << "_p" << (unsigned int)target_pos << "_";
    stringstream pathss1;
    pathss1 << pathss.str();

    pathss << FF_NAME << "_" << N_Generations << "_gens_" << pOn << ".csv";

    pathss1 << FF_NAME << "_" << N_Generations << "_gensplus_" << pOn << ".csv";

    f.open (pathss.str().c_str(), ios::out|ios::trunc);
    if (!f.is_open()) {
        cerr << "Error opening " << pathss.str() << endl;
        return 1;
    }

    f1.open(pathss1.str().c_str(), ios::out|ios::trunc);
    if (!f1.is_open()) {
        cerr << "Error opening " << pathss1.str() << endl;
        return 1;
    }

    for (unsigned int i = 0; i < generations.size(); ++i) {
        // One file has the time taken to get to F=1
        if (generations[i].fit == 1.0) {
            f << generations[i].gen_0 << endl;
        }
        // The other has the time (i.e. generations) taken to get to a fitness increment
        f1 << generations[i].gen << endl;
    }
    f.close();
    f1.close();

#ifdef RECORD_ALL_FITNESS
    // Preprocess the vector of vectors.

    // Find longest vector of fitnesses
    int maxevol = 0;
    int fs = 0;
    for (unsigned int i = 0; i < netinfo.size(); ++i) {
        maxevol = ((fs = netinfo[i].size()) > maxevol) ? fs : maxevol;
    }

    // Loop through all but the last entry in netinfo - these are most likely incomplete.
    for (unsigned int i = 0; i < (netinfo.size()-1); ++i) {
        if (!netinfo[i].empty()) {
            // Open file
            stringstream pathss2;
#ifdef STIPULATE_DRIFT_CASE
            pathss2 << "./data/evolutions/evolve_withf_";
#else
            pathss2 << "./data/evolutions/evolve_nodrift_withf_";
#endif
            pathss2 << "a" << (unsigned int)target_ant << "_p" << (unsigned int)target_pos << "_";
            pathss2 << FF_NAME << "_" << N_Generations <<  "_fitness_" << pOn
                    << "_genome_" << genome_id(netinfo[i].back().ab.genome) << ".csv";
            f.open (pathss2.str().c_str());
            if (!f.is_open()) {
                cerr << "Error opening " << pathss2.str() << endl;
                return 1;
            }

            // Output here. First we add the "pre-padding" so that all
            // fitness traces stored in this file have the same
            // length.

            // First line
            int counter = 1-maxevol;
            f << counter << ",0.0";
            f << "," << genome2str(netinfo[i][0].ab.genome);
            f << "," << netinfo[i][0].ab.getNumBasins();
            f << "," << netinfo[i][0].ab.meanAttractorLength();
            f << "," << netinfo[i][0].ab.maxAttractorLength();
            f << "," << netinfo[i][0].numChangedTransitions;
            f << "," << netinfo[i][0].deltaF;
            f << "," << N_Genes * (1<<N_Ins);
            f << endl;

            // Set up stringstream with the last line before the genome changes
            stringstream old_genome_ss;
            int lendiff = maxevol - netinfo[i].size();
            counter = lendiff-maxevol;
            old_genome_ss << counter << ",0.0";
            old_genome_ss << "," << genome2str(netinfo[i][0].ab.genome);
            old_genome_ss << "," << netinfo[i][0].ab.getNumBasins();
            old_genome_ss << "," << netinfo[i][0].ab.meanAttractorLength();
            old_genome_ss << "," << netinfo[i][0].ab.maxAttractorLength();
            old_genome_ss << "," << netinfo[i][0].numChangedTransitions;
            old_genome_ss << "," << netinfo[i][0].deltaF;
            old_genome_ss << ",0";
            old_genome_ss << endl;

            int lastgen = 0;
            double last_fitness = 0.0;
            // The last genome at which there was a fitness increment.
            array<genosect_t, N_Genes> last_genome = netinfo[i][0].ab.genome;
            // Now the actual data.
            lastgen = static_cast<int>(netinfo[i].back().generation);
            for (unsigned int j = 0; j < netinfo[i].size(); ++j) {
                if (netinfo[i][j].fitness > last_fitness) {
                    f << old_genome_ss.str();
                    // New genome:
                    f << static_cast<int>(netinfo[i][j].generation)-lastgen << "," << netinfo[i][j].fitness;
                    f << "," << genome2str(netinfo[i][j].ab.genome);
                    f << "," << netinfo[i][j].ab.getNumBasins();
                    f << "," << netinfo[i][j].ab.meanAttractorLength();
                    f << "," << netinfo[i][j].ab.maxAttractorLength();
                    f << "," << netinfo[i][j].numChangedTransitions;
                    f << "," << netinfo[i][j].deltaF;
                    f << "," << compute_hamming (last_genome, netinfo[i][j].ab.genome);
                    f << endl;
                    last_fitness = netinfo[i][j].fitness;
                    last_genome = netinfo[i][j].ab.genome;
                }

                old_genome_ss.str("");
                old_genome_ss.clear();
                old_genome_ss << static_cast<int>(netinfo[i][j].generation)-lastgen << "," << netinfo[i][j].fitness;
                old_genome_ss << "," << genome2str(netinfo[i][j].ab.genome);
                old_genome_ss << "," << netinfo[i][j].ab.getNumBasins();
                old_genome_ss << "," << netinfo[i][j].ab.meanAttractorLength();
                old_genome_ss << "," << netinfo[i][j].ab.maxAttractorLength();
                old_genome_ss << "," << netinfo[i][j].numChangedTransitions;
                old_genome_ss << "," << netinfo[i][j].deltaF;
                old_genome_ss << ",0";
                old_genome_ss << endl;
            }

            f.close();
        }
    }
#endif

    return 0;
}
