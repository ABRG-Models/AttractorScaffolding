/*
 * Find the proportion of fit genomes by randomly sampling the genome
 * space.
 *
 * This defaults the N_Genes=5, but N_Genes can be defined on the
 * compiler command line to find the results for other values.
 *
 * This code will do an exhaustive search if appropriate. In practice
 * this is possible for N_Genes==3 and k=n and N_Genes==4 and k=n-1
 * only (the latter requiring about 30 minutes of compute time).
 *
 * Author: S James
 * Date: October 2018.
 */

#include <iostream>
#include <vector>
#include <set>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// Choose debugging level.
//
// #define DEBUGF 1
// #define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#ifndef N_Genes
# define N_Genes 5
#endif

// Common code
#include "lib.h"

// The fitness function used here
#include "fitness.h"

#include "lmp.h"

#include <climits>

// Define a macro for evaluating the fitness to avoid writing it out twice
#define EVALUATE_FITNESS(genome_) {                                     \
        double f = evaluate_fitness (genome_);                          \
        if (f > 0.0) {                                                  \
            ++numfit;                                                   \
            if (f == 1.0) {                                             \
                ++numperfect;                                           \
            }                                                           \
        }                                                               \
        if (g%1000000 == 0) {                                           \
            cout << g << "," << numfit << "," << numperfect << endl;    \
        }                                                               \
        ++g;                                                            \
    }

int main (int argc, char** argv)
{
    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);
    // Set up the Mersenne Twister RNG
    rngDataInit (&rd);
    zigset (&rd, DUMMYARG);
    rd.seed = seed;

    // Initialise masks
    masks_init();

    // Unused, but set, in this program.
    pOn = 0.5;

    // To store the results of this program
    unsigned long long int numfit = 0;
    unsigned long long int numperfect = 0;

    // Compute the dimensionality of the genome space
    unsigned long long int N = (N_Genes * (1ULL << N_Ins));
    LOG ("Genome space has N = " << N << " dimensions");

    // Find out how many genomes we'd have to test to do an exhaustive search
    Xint nexhaustive_xint, tmp1;
    lmp::InitSetUi (nexhaustive_xint, 1);
    lmp::InitSetUi (tmp1, 2);
    for (unsigned long long int i = N; i > 0; i--) {
        lmp::Mul (nexhaustive_xint, nexhaustive_xint, tmp1);
    }

    // Try to place the exhaustive search number into a 64 bit unsigned integer
    unsigned long long int nexhaustive = 0;
    stringstream nexss;
    nexss << nexhaustive_xint;
    nexss >> nexhaustive;

    LOG ("2^N = " << nexhaustive_xint << " or " << nexhaustive << " (cf " << ULLONG_MAX << ")");

    // Could be set on cmd line. This many takes 30 mins for n=4 and
    //ensures I can do an exhaustive search for n=4 and k=n=1
    //unsigned long long int ntrials = 4294967296;

    // This is about 3 minutes worth of computation:
    unsigned long long int ntrials = 400000000;

    if (nexhaustive != ULLONG_MAX && nexhaustive <= ntrials) {
        LOG ("Doing an exhaustive search...");
        ntrials = nexhaustive;
        // Create genome, initialise to 0:
        array<genosect_t, N_Genes> genome;
        zero_genome (genome);
        // A counter for the number of trials; should end up equal to ntrials:
        unsigned long long int g = 0;
        // Number of permutations in a genome section:
        unsigned long long int genome_sect_perms = (0x1ULL << (0x1UL<<N_Ins));
#if N_Genes == 3
        for (unsigned long long int g1 = 0; g1 < genome_sect_perms; ++g1) {
            for (unsigned long long int g2 = 0; g2 < genome_sect_perms; ++g2) {
                for (unsigned long long int g3 = 0; g3 < genome_sect_perms; ++g3) {
                    //DBG2 ("Evaluate genome " << genome_id(genome));
                    EVALUATE_FITNESS(genome);
                    genome[2]++;
                }
                genome[2] = 0;
                genome[1]++;
            }
            genome[2] = 0;
            genome[1] = 0;
            genome[0]++;
        }
#elif N_Genes == 4 // Running the program for N_Genes==4 will only be tractable for the n=k-1 case.
        for (unsigned long long int g1 = 0; g1 < genome_sect_perms; ++g1) {
            for (unsigned long long int g2 = 0; g2 < genome_sect_perms; ++g2) {
                for (unsigned long long int g3 = 0; g3 < genome_sect_perms; ++g3) {
                    for (unsigned long long int g4 = 0; g4 < genome_sect_perms; ++g4) {
                        EVALUATE_FITNESS(genome);
                        genome[3]++;
                    }
                    genome[3] = 0;
                    genome[2]++;
                }
                genome[3] = 0;
                genome[2] = 0;
                genome[1]++;
            }
            genome[3] = 0;
            genome[2] = 0;
            genome[1] = 0;
            genome[0]++;
        }
#else
        LOG ("Uh oh; I only have code for computing an exhaustive search for N_Genes == 3 or N_Genes==4.");
#endif
        LOG ("At end g was " << g << ". Each loop was of length " << genome_sect_perms);

    } else {
        LOG ("Doing sampling search...");
        for (unsigned long long int g = 0; g < ntrials; ) {
            array<genosect_t, N_Genes> genome = random_genome();
            // Note: g increments in EVALUATE_FITNESS():
            EVALUATE_FITNESS(genome);
        }
    }

    cout << ntrials << "," << numfit << "," << numperfect << endl;

    LOG ("For " << N_Genes << " genes, there were " << numfit << " fit genomes out of " << ntrials << " of which " << numperfect << " had F=1");
    LOG ("That's " << (double)numfit / ntrials << " and "<< (double)numperfect / ntrials << " as proportions.");
    LOG ("That's " << (100.0 * (double)numfit / ntrials) << "% and " << (100.0 * (double)numperfect / ntrials) << "%.");
    return 0;
}
