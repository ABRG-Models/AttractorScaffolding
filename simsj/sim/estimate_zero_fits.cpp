/*
 * Randomly selects N_Genomes genomes and finds numbers for those with
 * fitness 0, those with >0 fitness and those with fitness == 1.
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

// The number of genomes to generate
#define N_Genomes 1000000

// Common code
#include "lib.h"

// The fitness function used here
#include "fitness.h"

// Perform a loop N_Generations long during which an initially
// randomly selected genome is evolved until a maximally fit state is
// achieved.
int main (int argc, char** argv)
{
    // Seed the RNG
    srand(5);

    // Initialise masks
    masks_init();

    unsigned int num_fitness1    = 0;
    unsigned int num_fitness0    = 0;
    unsigned int num_fitness_gt0 = 0;

    array<genosect_t, N_Genes> genome;
    for (unsigned int i = 0; i < N_Genomes; ++i) {
        random_genome (genome);
        float f = evaluate_fitness (genome);
        if (f > 1.0) {
            cerr << "Fitness > 1.0?!? [" << f << "]" << endl;
            return 1;
        } else if (f == 1.0) {
            ++num_fitness1;
            ++num_fitness_gt0;
        } else if (f > 0.0) {
            ++num_fitness_gt0;
        } else if (f == 0.0) {
            ++num_fitness0;
        } else {
            cerr << "Fitness < 0.0?!? [" << f << "]" << endl;
            return 1;
        }
    }

    LOG (num_fitness1 << "/" << N_Genomes << " genomes had fitness 1");
    LOG (num_fitness_gt0 << "/" << N_Genomes << " genomes had fitness >0");
    LOG (num_fitness0 << "/" << N_Genomes << " genomes had fitness 0");

    return 0;
}
