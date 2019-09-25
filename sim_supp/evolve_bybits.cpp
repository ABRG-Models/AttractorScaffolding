/*
 * Evolves genome repeatedly according to the fitness function
 * described in the paper associated with this code.
 *
 * This version uses the evolve_genome() function that flips a set
 * number of bits, rather than flipping each by probability
 *
 * Author: S James
 * Date: December 2018.
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
        LOG ("Usage: " << argv[0] << " bits_to_flip target_ant target_pos");
        LOG ("   or: " << argv[0] << " bits_to_flip nGenerations");
        LOG ("Supply the number of bits to flip in the gene during evolution, bits_to_flip (uint)");
        LOG ("Optionally supply the anterior and posterior targets (integer, in range 0 to 31) or number of generations to run for.");
        return 1;
    }
    unsigned int bits_to_flip = static_cast<unsigned int>(atoi (argv[1]));
    LOG ("Number of bits to flip each generation = " << bits_to_flip);

    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);
    // Set up the Mersenne Twister RNG
    rngDataInit (&rd);
    zigset (&rd, DUMMYARG);
    rd.seed = seed;

    // Initialise masks
    masks_init();

    unsigned long long int nGenerations = N_Generations;
    if (argc == 3) {
        nGenerations = static_cast<unsigned long long int>(atoi (argv[2]));
    }

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
    // A vector of NetInfo to populate between fitness increments,
    // then clear. Prevents netinfo from consuming too much RAM.
    NetInfo ni;
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

    unsigned long long int f1count = 0;

    while (gen < nGenerations) {

        // At the start of the loop, and every time fitness of 1.0 is
        // achieved, generate a random genome starting point.
        random_genome (refg);

        // Make a copy of the genome, in case evolving it leads to a
        // less fit genome, then evaluate the fitness of the genome.
        double a = evaluate_fitness (refg);

#ifdef RECORD_ALL_FITNESS
        AllBasins ab_a (refg);
        ni.update(ab_a, gen, a);
        ni.deltaF = 0.0;
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
//#define REDUCING_BIT_FLIPS_AS_F_INCREASES 1
#ifdef REDUCING_BIT_FLIPS_AS_F_INCREASES // Just an idea
            unsigned int modflips = (unsigned int) ceil((1.0-a) * bits_to_flip);
            DBG2 ("Fitness: " << a << " bits to flip: " << modflips);
            evolve_genome (newg, modflips);
#else
            evolve_genome (newg, bits_to_flip);
#endif
#ifdef RECORD_ALL_FITNESS
            AllBasins ab2 (newg);
            set<unsigned int> diffs;
            set_difference (ab1.transitions.begin(), ab1.transitions.end(),
                            ab2.transitions.begin(), ab2.transitions.end(),
                            inserter(diffs, diffs.begin()));
#endif
            ++gen; // Because we evolved

            if (gen > 0 && (gen % N_Genview == 0)) {
                LOG ("[flipbits: " << bits_to_flip << "] That's " << gen/1000000.0 << "M generations (out of "
                     << nGenerations/1000000.0 << "M) done...");
            }

            if (gen >= nGenerations) {
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
                //NetInfo ni(ab_a, gen, a);
                ni.update(ab_a, gen, a);
                ni.deltaF = 0.0;
                //netinfo.back().push_back (ni);
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
                NetInfo niinc (ab_b, gen, b);
                niinc.numChangedTransitions = difference.size();
                niinc.deltaF = static_cast<double>(b - a);
                netinfo.back().push_back (ni);
                netinfo.back().push_back (niinc);
#endif
                // Record the fitness increase in generations:
                generations.push_back (geninfo(gen-lastgen, gen-lastf1, b));
                lastgen = gen;
                if (b==1.0) {
                    lastf1 = gen;
                    DBG ("F=1 at generation " << gen);
                    ++f1count;
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
        if (gen < nGenerations) {
            vector<NetInfo> vni;
            netinfo.push_back (vni);
        }
        // Plus also analyse the basins of attraction and save this
        // information. The most compact way to save this information
        // is simply to save the genome.
#endif
    }

    LOG ("Generations size: " << generations.size() << " with " << f1count << " F=1 genomes found.");

    // Save data to file.
    ofstream f, f1;
    stringstream pathss;
    pathss << "./data/";
#ifdef RECORD_ALL_FITNESS
    pathss << "evolutions/";
#endif
#ifdef REDUCING_BIT_FLIPS_AS_F_INCREASES
    pathss << "r_";
#endif
#ifdef STIPULATE_DRIFT_CASE
    pathss << "evolveb_";
#else
    pathss << "evolveb_nodrift_";
#endif
#ifdef RECORD_ALL_FITNESS
    pathss << "withf_";
#endif
    pathss << "a" << (unsigned int)target_ant << "_p" << (unsigned int)target_pos << "_";
    stringstream pathss1;
    pathss1 << pathss.str();

    pathss << FF_NAME << "_" << nGenerations << "_gens_" << bits_to_flip << ".csv";

    pathss1 << FF_NAME << "_" << nGenerations << "_gensplus_" << bits_to_flip << ".csv";

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
        fs = netinfo[i].back().generation - netinfo[i].front().generation;
        maxevol = ((fs > maxevol) ? fs : maxevol);
        LOG ("fs: " << fs << ", maxevol: " << maxevol);
    }

    // Loop through all but the last entry in netinfo - these are most likely incomplete.
    for (unsigned int i = 0; i < (netinfo.size()-1); ++i) {
        if (!netinfo[i].empty()) {
            // Open file
            stringstream pathss2;
#ifdef STIPULATE_DRIFT_CASE
            pathss2 << "./data/evolutions/evolveb_withf_";
#else
            pathss2 << "./data/evolutions/evolveb_nodrift_withf_";
#endif
            pathss2 << "a" << (unsigned int)target_ant << "_p" << (unsigned int)target_pos << "_";
            pathss2 << FF_NAME << "_" << nGenerations <<  "_fitness_" << bits_to_flip
                    << "_genome_" << genome_id(netinfo[i].back().ab.genome) << ".csv";
            f.open (pathss2.str().c_str());
            if (!f.is_open()) {
                cerr << "Error opening " << pathss2.str() << endl;
                return 1;
            }

            // Output here. First we add the "pre-padding" so that all
            // fitness traces stored in this file have the same
            // length.
            if (netinfo[i].back().generation < (unsigned int)maxevol) {
                LOG ("Here goes outputting dummy point at gen==" << (-maxevol));
                // Output a dummy point at -maxevol
                f << -maxevol
                  << "," << netinfo[i][0].fitness
                  << "," << genome2str(netinfo[i][0].ab.genome)
                  << "," << netinfo[i][0].ab.getNumBasins()
                  << "," << netinfo[i][0].ab.meanAttractorLength()
                  << "," << netinfo[i][0].ab.maxAttractorLength()
                  << "," << netinfo[i][0].numChangedTransitions
                  << ",0"
                  << ",0"
                  << endl;
            }

            array<genosect_t, N_Genes> last_genome = netinfo[i][0].ab.genome;
            long long int lgen = (long long int)netinfo[i].back().generation;
            for (unsigned int j = 0; j < netinfo[i].size(); ++j) {
                f << ((long long int)netinfo[i][j].generation - lgen)
                  << "," << netinfo[i][j].fitness
                  << "," << genome2str(netinfo[i][j].ab.genome)
                  << "," << netinfo[i][j].ab.getNumBasins()
                  << "," << netinfo[i][j].ab.meanAttractorLength()
                  << "," << netinfo[i][j].ab.maxAttractorLength()
                  << "," << netinfo[i][j].numChangedTransitions
                  << "," << netinfo[i][j].deltaF
                  << "," << compute_hamming (last_genome, netinfo[i][j].ab.genome)
                  << endl;
                last_genome = netinfo[i][j].ab.genome;
            }
        }
        f.close();
    }
#endif

    return 0;
}
