/*
 * Take N_Starts randomly generated, but f>0 genomes. Mutate each one
 * N_Generations times. Determine the probability distribution of
 * mutating to a fitter genome a value of pOn provided on the command
 * line.
 *
 * Author: S James
 * Date: November 2018.
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

// The number of starting genomes to generate
#ifndef N_Starts
# define N_Starts 5000
#endif

// The number of times to evolve from each starting genome
#ifndef N_Generations
# define N_Generations 50000
#endif

// Common code
#include "lib.h"

// The fitness function used here
#include "fitness.h"

// Perform a loop N_Generations long during which an initially
// randomly selected genome is evolved until a maximally fit state is
// achieved.
int main (int argc, char** argv)
{
    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);

    // Initialise masks
    masks_init();

    // Set pOn using the command line.
    if (argc < 2) {
        cerr << "Examines the statistics of the probability of evolving to" << endl
             << "a higher fitness genome for " << N_Starts << " starting genomes." << endl
             << "Uses the fitness function " << FF_NAME << " and a given probability " << endl
             << "of flipping (pOn) which must be supplied." << endl << endl
             << "Usage: " << argv[0] << " pOn" << endl;
        return 1;
    }
    pOn = atof (argv[1]);

    // Holds the starting genomes
    array<array<genosect_t, N_Genes>, N_Starts> genomes;

    // For each starting genome, record the number that evolved to a
    // fitter genome and also the number than evolve to an equally fit
    // genome.
    array<unsigned int, N_Starts> numFitter;
    array<unsigned int, N_Starts> numEqual;
    // Store the fitnesses of all the starting genomes
    array<double, N_Starts> initialFitness;
    // Explicitly init to 0
    for (unsigned int g = 0; g < N_Starts; ++g) {
        numFitter[g] = 0;
        numEqual[g] = 0;
        initialFitness[g] = 0.0;
    }

    // Initialise genomes
    for (unsigned int g = 0; g < N_Starts; ++g) {
        while (initialFitness[g] == 0.0) {
            random_genome (genomes[g]);
            initialFitness[g] = evaluate_fitness (genomes[g]);
        }
    }

    // Experiment with the probability of evolving fitter
    for (unsigned int g = 0; g < N_Starts; ++g) {
        for (unsigned int e = 0; e < N_Generations; ++e) {
            array<genosect_t, N_Genes> testg;
            copy_genome (genomes[g], testg);
            evolve_genome (testg);
            double newf = evaluate_fitness (testg);
            if (newf > initialFitness[g]) {
                // Fitness increased
                ++numFitter[g];
            } else if (newf == initialFitness[g]) {
                ++numEqual[g];
            } // else new fitness is lower than initial fitness
        }
    }

    cout << "Collected data; writing out..." << endl;
    ofstream fout;
    stringstream path;
    path << "data/prob_evolvefitter_" << N_Generations << "_evolutions_pOn_" << pOn << ".csv";
    fout.open (path.str().c_str(), ios::out|ios::trunc);
    if (!fout.is_open()) {
        cerr << "Failed to open " << path.str() << " for writing." << endl;
        return 1;
    }

    fout << "GenomeID,Fitness,NumberEvolvingFitter,NumberEvolvingEqual" << endl;
    for (unsigned int g = 0; g < N_Starts; ++g) {
        fout << genome_id(genomes[g]) << "," << initialFitness[g] << "," << numFitter[g] << "," << numEqual[g] << endl;
    }

    fout.close();

    return 0;
}
