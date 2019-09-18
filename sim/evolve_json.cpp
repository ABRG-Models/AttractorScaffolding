/*
 * Evolves genome repeatedly according to the fitness function described in the paper associated
 * with this code.
 *
 * This version of the program takes parameter info from a JSON file.
 *
 * Additionally, this version of the program (will be) is able to evolve towards any number of
 * target state contexts. The original version (including the standalone version) was constrained
 * to work with 2 contexts; 'anterior' and 'posterior'.
 *
 * Author: S James
 * Date: September 2019.
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

// Choose debugging level.
//
// #define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// The number of generations to evolve for by default, unless otherwise specfied in JSON
#define N_Generations   100000000

// I'm using JSON to read in simulation parameters from a config file, following our new lab
// policy.
#include <json/json.h>

// Whether to create large files with all the fitness information, which can be used to make up
// graphs showing evolution of the fitness.
//
// #define RECORD_ALL_FITNESS 1

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

// Perform a loop N_Generations long during which an initially randomly selected genome is evolved
// until a maximally fit state is achieved.
int main (int argc, char** argv)
{
    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);

    // Initialise masks
    masks_init();

    // Get JSON parameter config path
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " /path/to/params.json [pOn]" << endl;
        cerr << "       (pOn may be specified to override any value obtained from the JSON)"
            << endl;
        return 1;
    }
    string paramsfile (argv[1]);

    // Allow the pOn to be specified on the command line, overriding the JSON config, for script
    // simplicity
    float pOnCmd = -1.0f;
    if (argc >= 3) {
        pOnCmd = static_cast<float>(atof (argv[2]));
        LOG ("pOn in JSON will be overridden to " << pOnCmd);
    }

    // Test for existence of the JSON file
    ifstream jsonfile_test;
    int srtn = system ("pwd");
    if (srtn) { cerr << "system call returned " << srtn << endl; }
    jsonfile_test.open (paramsfile, ios::in);
    if (jsonfile_test.is_open()) {
        // Good, file exists.
        jsonfile_test.close();
    } else {
        cerr << "json config file " << paramsfile << " not found." << endl;
        return 1;
    }
    // Parse the JSON
    ifstream jsonfile (paramsfile, ifstream::binary);
    Json::Value root;
    string errs;
    Json::CharReaderBuilder rbuilder;
    rbuilder["collectComments"] = false;
    bool parsingSuccessful = Json::parseFromStream (rbuilder, jsonfile, &root, &errs);
    if (!parsingSuccessful) {
        // report to the user the failure and their locations in the document.
        cerr << "Failed to parse JSON: " << errs;
        return 1;
    }

    // Set up simulation parameters from JSON
    if (pOnCmd > -1.0f) {
        pOn = pOnCmd;
    } else {
        pOn = root.get ("pOn", 0.5).asFloat();
    }
    const unsigned long long int nGenerations =
        static_cast<unsigned long long int>(root.get ("nGenerations", N_Generations).asUInt64());
    const unsigned int nGenView = root.get ("nGenerations", N_Generations/100).asUInt();
    {
        state_t ta = static_cast<state_t>(root.get ("target_ant", target_ant).asUInt());
        if (ta != target_ant) {
            target_ant = ta;
        }
        state_t tp = static_cast<state_t>(root.get ("target_pos", target_pos).asUInt());
        if (tp != target_pos) {
            target_pos = tp;
        }
        ta = static_cast<state_t>(root.get ("initial_ant", initial_ant).asUInt());
        if (ta != initial_ant) {
            initial_ant = ta;
        }
        tp = static_cast<state_t>(root.get ("initial_pos", initial_pos).asUInt());
        if (tp != initial_pos) {
            initial_pos = tp;
        }
    }
    const bool drift = root.get ("drift", true).asBool();
    const string logdir = root.get ("logdir", "./data").asString();
    // Done getting params
    LOG ("pOn:" << pOn
         << "; initial a/p: " << state_str(initial_ant) << "/" << state_str(initial_pos)
         << "; target a/p: " << state_str(target_ant) << "/" << state_str(target_pos));
    if (drift == false) {
        LOG ("Running the 'no drift' algorithm and saving data into " << logdir);
    } else {
        LOG ("Saving data into " << logdir);
    }

    // generations records the relative generation number, and the fitness. Every entry in this
    // records an increase in the fitness of the genome.
    vector<geninfo> generations;

#ifdef RECORD_ALL_FITNESS
    // Records the evolution of the fitness of a genome. Fig 3. The (abs) generation for each
    // fitness is recorded along with the floating point fitness value. Record this in a vector of
    // vectors, with one vector for each evolution towards F=1
    vector<vector<NetInfo> > netinfo;
    vector<NetInfo> ni0;
    netinfo.push_back (ni0);
    // A vector of NetInfo to populate between fitness increments, then clear. Prevents netinfo
    // from consuming too much RAM.
    NetInfo ni;
#endif

    // Holds the genome and a copy of it.
    array<genosect_t, N_Genes> refg;
    array<genosect_t, N_Genes> newg;

    // The main loop. Repeatedly evolve from a random genome starting point, recording the number
    // of generations required to achieve a maximally fit state of 1.
    unsigned long long int gen = 0;
    unsigned long long int lastgen = 0;
    unsigned long long int lastf1 = 0;

    // Count F=1 genomes to print out at the end.
    unsigned long long int f1count = 0;

    while (gen < nGenerations) {

        // At the start of the loop, and every time fitness of 1.0 is achieved, generate a random
        // genome starting point.
        random_genome (refg);

        // Make a copy of the genome, in case evolving it leads to a less fit genome, then
        // evaluate the fitness of the genome.
        double a = evaluate_fitness (refg);

        // a randomly selected genome can be maximally fit
        if (a==1.0) {
            generations.push_back (geninfo(gen-lastgen, gen-lastf1, a));
            lastgen = gen;
            lastf1 = gen;
            ++f1count;
        }

#ifdef RECORD_ALL_FITNESS
        AllBasins ab_a (refg);
        ni.update(ab_a, gen, a);
        ni.deltaF = 0.0;
#endif
        //LOG ("New random genome generated with fitness:" << a);
        ++gen; // Because we randomly generated.

        // Note that if a==1.0 after the call to random_genome(), we should cycle around and call
        // random_genome() again.

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

            if (gen > 0 && (gen % nGenView == 0)) {
                LOG ("[pOn=" << pOn << "] That's " << gen/1000000.0 << "M generations (out of "
                     << nGenerations/1000000.0 << "M) done...");
            }

            if (gen >= nGenerations) {
                break;
            }
            double b = evaluate_fitness (newg);

            // DRIFT: New fitness < old fitness; NO DRIFT: New fitness <= old fitness
            if (drift ? b < a : b <= a) {
#ifdef RECORD_ALL_FITNESS
                // Record _existing_ fitness f, not new fitness.
                ni.update(ab_a, gen, a);
                ni.deltaF = 0.0;
#endif
                // ...but otherwise, do nothing.
            } else {
#ifdef RECORD_ALL_FITNESS
                DBG2 ("New fitness is greater than old fitness! Fitness:" << b
                      << " differences since refg: " << diffs.size());
                // Record new fitness, even if a==b - this is the "drift case".  Also examine the
                // new basins of attraction and then compare ab_a and ab_b.
                AllBasins ab_b (newg);
                // Find the difference in the transitions defined by the two different genomes
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
        // Plus also analyse the basins of attraction and save this information. The most compact
        // way to save this information is simply to save the genome.
#endif
    }

    LOG ("Generations size: " << generations.size()
         << " with " << f1count << " F=1 genomes found.");

    // Save data to file.
    ofstream f, f1;
    stringstream pathss;
    pathss << logdir << "/";
#ifdef RECORD_ALL_FITNESS
    pathss << "evolutions/";
#endif
    if (drift == true) {
        pathss << "evolve_";
    } else {
        pathss << "evolve_nodrift_";
    }
#ifdef RECORD_ALL_FITNESS
    pathss << "withf_";
#endif
    pathss << "ia" << (unsigned int)initial_ant << "_ip" << (unsigned int)initial_pos << "_";
    pathss << "a" << (unsigned int)target_ant << "_p" << (unsigned int)target_pos << "_";
    stringstream pathss1;
    pathss1 << pathss.str();

    pathss << FF_NAME << "_" << nGenerations << "_gens_" << pOn << ".csv";

    pathss1 << FF_NAME << "_" << nGenerations << "_gensplus_" << pOn << ".csv";

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
            // Open the file
            stringstream pathss2;
            if (drift == true) {
                pathss2 << logdir << "/evolutions/evolve_withf_";
            } else {
                pathss2 << logdir << "/evolutions/evolve_nodrift_withf_";
            }
            pathss2 << "ia" << (unsigned int)initial_ant
                    << "_ip" << (unsigned int)initial_pos << "_";
            pathss2 << "a" << (unsigned int)target_ant << "_p" << (unsigned int)target_pos << "_";
            pathss2 << FF_NAME << "_" << nGenerations <<  "_fitness_" << pOn
                    << "_genome_" << genome_id(netinfo[i].back().ab.genome) << ".csv";
            f.open (pathss2.str().c_str());
            if (!f.is_open()) {
                cerr << "Error opening " << pathss2.str() << endl;
                return 1;
            }

            // Output into the file. First we add the "pre-padding" so that all fitness traces
            // stored in this file have the same length.
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
