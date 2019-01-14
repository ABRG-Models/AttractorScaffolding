/*
 * Standalone evolve program.
 *
 * Evolves genome repeatedly according to the fitness function
 * described in the paper associated with this code.
 *
 * Author: S James
 * Date: 14th January 2019.
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
#include <array>
#include <list>
#include <math.h>
#include <immintrin.h> // Using intrinsics for computing Hamming distances
#include <set>

using namespace std;

// A macro for logging to stdout
#define LOG(s)  cout << "LOG: " << s << endl;

// Number of genes in a state is set at compile time.
#define N_Genes 5

// The number of generations to evolve for. 100,000,000 takes roughly
// 3 minutes and 30 seconds on an Intel Core i9-8950HK CPU, for n=5
// and k=n.
#define N_Generations   100000000

// How often to make a message print out about progress. Make 10 or
// 100 times smaller than N_Generations.
#define N_Genview       1000000

#define ExtraOffset  (1)
#define N_Ins        (N_Genes)

/*!
 * The genome has a section for each gene. The length of the
 * section of each gene is 2^N_Ins == 2^5 == 32.
 */
typedef unsigned int genosect_t;
#define GENOSECT_ONE 0x1UL

/*!
 * The state has N_Genes bits in it. Working with N_Genes <= 8, so:
 */
typedef unsigned char state_t;

/*!
 * When right-shifting the hi_mask, we need to set the top bit to 1,
 * because right-shifting an unsigned integer number always zero-fills
 * by default.
 */
#define state_t_top_bit 0x80

/*!
 * Probability of flipping each bit of the genome during evolution.
 */
float pOn;

/*!
 * Starting values of the masks used when computing inputs from a
 * state. When computing input for a gene at position i, the hi_mask
 * is used to obtain inputs for positions > i, the low mask for
 * position < i. The hi bits are then shifted right once to make up an
 * input containing N_Genes-1 bits. Must be set up using masks_init().
 */
//@{
unsigned char lo_mask_start;
unsigned char hi_mask_start;
//@}

/*!
 * The mask used to get the significant bits of genome section. Set up
 * using masks_init().
 */
genosect_t genosect_mask;

/*!
 * The mask used to get the significant bits of a state. Set up using
 * masks_init().
 */
state_t state_mask;

/*!
 * Set the global target states for anterior and posterior positions.
 */
//@{
state_t target_ant = 0x15; // 10101 or 21 dec
state_t target_pos = 0xa;  // 01010 or 10 dec
//@}

/*!
 * Initial anterior and posterior states:
 */
//@{
state_t initial_ant = 0x10; // 10000b;
state_t initial_pos = 0x0;  // 00000b;
//@}

/*!
 * Initialise the masks based on the value of N_Genes
 */
void
masks_init (void)
{
    // Set up globals. Set N_Ins bits to the high position for the lo_mask
    lo_mask_start = 0x0;
    for (unsigned int i = 0; i < N_Ins; ++i) {
        lo_mask_start |= 0x1 << i;
    }
    hi_mask_start = 0xff & (0xff << N_Genes);

    genosect_mask = 0x0;
    for (unsigned int i = 0; i < (1<<N_Ins); ++i) { // 1<<N is the same as 2^N
        genosect_mask |= (0x1 << i);
    }

    state_mask = 0x0;
    for (unsigned int i = 0; i < N_Genes; ++i) {
        state_mask |= (0x1 << i);
    }
}

/*!
 * Given a state for N_Genes, and a genome, compute the next
 * state. This is "develop" rather than "evolve".
 */
void
compute_next (const array<genosect_t, N_Genes>& genome, state_t& state)
{
    array<state_t, N_Genes> inputs;

    for (unsigned int i = 0; i < N_Genes; ++i) {
        inputs[i] = ((state << i) & state_mask) | (state >> (N_Genes-i));
    }

    // Now reset state and compute new values:
    state = 0x0;

    // State a anterior is genome[inps[0]] etc
    for (unsigned int i = 0; i < N_Genes; ++i) {
        // Setting state for gene i
        genosect_t gs = genome[i];
        genosect_t inpit = (0x1 << inputs[i]);
        state_t num = ((gs & inpit) ? 0x1 : 0x0);
        if (num) {
            state |= (0x1 << (N_Ins-(i+ExtraOffset)));
        } else {
            state &= ~(0x1 << (N_Ins-(i+ExtraOffset)));
        }
    }
}

/*!
 * Generate a string representation of the state. Something like "1 0 1
 * 1 1" or "0 0 1 1 0".
 */
string
state_str (const state_t& state)
{
    stringstream ss;
    // Count down from N_Genes, to output bits in order MSB to LSB.
    for (unsigned int i = N_Genes; i > 0; --i) {
        unsigned int j = i-1;
        ss << ((state & (0x1<<j)) >> j) << " ";
    }
    return ss.str();
}

/*!
 * For mixing up bits of three args; used to generate a good random
 * seed using time() getpid() and clock().
 */
unsigned int
mix (unsigned int a, unsigned int b, unsigned int c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

/*!
 * Return a random single precision number between 0 and 1.
 */
float
randFloat (void)
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

/*!
 * Return a random double precision number between 0 and 1.
 */
double
randDouble (void)
{
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

/*!
 * Copy the contents of @from to @to
 */
void
copy_genome (const array<genosect_t, N_Genes>& from, array<genosect_t, N_Genes>& to)
{
//#pragma omp simd
    for (unsigned int i = 0; i < N_Genes; ++i) {
        to[i] = from[i];
    }
}

/*!
 * The evolution function. Note that this function depends on the
 * existence of a global variable pOn.
 */
void
evolve_genome (array<genosect_t, N_Genes>& genome)
{
    for (unsigned int i = 0; i < N_Genes; ++i) {
        genosect_t gsect = genome[i];
        for (unsigned int j = 0; j < (1<<N_Ins); ++j) {
            if (randDouble() < pOn) {
                // Flip bit j
                gsect ^= (GENOSECT_ONE << j);
            }
        }
        genome[i] = gsect;
    }
}

/*!
 * Populate the passed in genome with random bits.
 */
void
random_genome (array<genosect_t, N_Genes>& genome)
{
    for (unsigned int i = 0; i < N_Genes; ++i) {
        genome[i] = ((genosect_t) rand()) & genosect_mask;
    }
}

/*!
 * When working with states in a graph of nodes, it may be necessary
 * to use one bit to refer to the state as being unset; this is the
 * bit to use.
 */
#define state_t_unset 0x80

#define FF_NAME "ff4"

/*!
 * Evaluates the fitness of one context (anterior or posterior).
 */
double
evaluate_one (array<genosect_t, N_Genes>& genome, state_t state, state_t target)
{
    double score = 0.0;

    state_t state_last = state_t_unset;
    set<state_t> visited;
    visited.insert (state); // insert starting state
    for (;;) {
        state_last = state;
        compute_next (genome, state);

        if (visited.count (state)) {
            // Already visited this state so it's a limit cycle
            if (state == state_last) {
                // Point attractor
                if (state == target) {
                    score = 1.0;
                } // else score is definitely 0.

            } else {
                // Limit cycle

                // Determine the states in the limit cycle by going
                // around it once more.
                set<state_t> lc;
                unsigned int lc_len = 0;
                while (lc.count (state) == 0) {
                    // Check if we have one or both target states on
                    // this limit cycle
                    lc.insert (state);
                    lc_len++;
                    compute_next (genome, state);
                }

                // Now have the set lc; can work out its score.

                // For tabulating the scores
                array<double, N_Genes> sc;
                for (unsigned int j = 0; j < N_Genes; ++j) { sc[j] = 0.0; }

                set<state_t>::const_iterator i = lc.begin();
                while (i != lc.end()) {
                    state_t a = ((*i) ^ ~target) & state_mask;
                    for (unsigned int j = 0; j < N_Genes; ++j) {
                        sc[j] += static_cast<double>( (a >> j) & 0x1 );
                    }
                    ++i;
                }
                // Divide down now.
//#pragma omp simd
                for (unsigned int j = 0; j < N_Genes; ++j) {
                    sc[j] /= static_cast<double>(lc_len);
                }

                score = sc[0];
                for (unsigned int j = 1; j < N_Genes; ++j) {
                    score = score * sc[j];
                }
            }
            break;
        }
        visited.insert (state);
    }

    return score;
}

/*!
 * For the passed-in genome, find its final state, starting from the
 * anterior state initial_ant and the posterior state initial_pos
 * (stored in global variables). Return a fitness specifier for the
 * genome.
 *
 * This function examines the limit cycle that is arrived at from the
 * two initial states. The mean value of each bit in the limit cycle
 * is compared with the target state.
 *
 * The fitness is then computed according
 * to:
 *
 * f = (a0 * a1 * a2 * a3 * a4) * (p0 * p1 * p2 * p3 * p4)
 *
 * a0 is the proportion of time during the limit cycle that bit 0 has
 * the state matching the anterior target
 *
 * Returns fitness in range 0 to 1.0. Note use of double. The fitness
 * values can potentially be very small for a long limit cycle. For
 * example, for a 5 gene LC of size 10, the fitness could be as low as
 * (1/10)^5 * (1/10)^5 = 1/10^10, which is heading towards what a
 * single precision float can represent.
 *
 * For further details on this fitness evaluation, please see the
 * associated paper.
 */
double
evaluate_fitness (array<genosect_t, N_Genes>& genome)
{
    double ant_score = evaluate_one (genome, initial_ant, target_ant);
    double pos_score = evaluate_one (genome, initial_pos, target_pos);
    double fitness = ant_score * pos_score;
    return fitness;
}

/*!
 * A data structure to record information about the fitness increments.
 */
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

// For pOn=0.1 to pOn=0.5, perform a loop N_Generations long during
// which an initially randomly selected genome is evolved until a
// maximally fit state is achieved. Once the f=1 state is achieved,
// the genome is re-randomised and the evolution continues.
int main (int argc, char** argv)
{
    // Initialise masks
    masks_init();

    // Seed the RNG.
    unsigned int seed = mix(clock(), time(NULL), getpid());
    srand (seed);

    unsigned long long int nGenerations = N_Generations;

    for (pOn = 0.1; pOn < 0.6;  pOn += 0.1) {

        LOG ("Computing " << nGenerations << " evolutions for p=" << pOn << "...");

        // generations records the relative generation number, and the
        // fitness. Every entry in this records an increase in the fitness
        // of the genome.
        vector<geninfo> generations;

        // Holds the genome and a copy of it.
        array<genosect_t, N_Genes> refg;
        array<genosect_t, N_Genes> newg;

        // The main loop. Repeatedly evolve from a random genome starting
        // point, recording the number of generations required to achieve a
        // maximally fit state of 1.
        unsigned long long int gen = 0;
        unsigned long long int lastgen = 0;
        unsigned long long int lastf1 = 0;

        // Count F=1 genomes to print out at the end.
        unsigned long long int f1count = 0;

        while (gen < nGenerations) {

            // At the start of the loop, and every time fitness of 1.0 is
            // achieved, generate a random genome starting point.
            random_genome (refg);

            // Make a copy of the genome, in case evolving it leads to a
            // less fit genome, then evaluate the fitness of the genome.
            double a = evaluate_fitness (refg);

            ++gen; // Because we randomly generated.

            // Note that if a==1.0 after the call to random_genome(), we
            // should cycle around and call random_genome() again.

            // Test fitness to determine whether we should evolve.
            while (a < 1.0) {

                copy_genome (refg, newg);
                evolve_genome (newg);
                ++gen; // Because we evolved

                if (gen > 0 && (gen % N_Genview == 0)) {
                    LOG ("[pOn=" << pOn << "] That's " << gen/1000000.0 << "M generations (out of "
                         << nGenerations/1000000.0 << "M) done...");
                }

                if (gen >= nGenerations) {
                    break;
                }
                double b = evaluate_fitness (newg);

                if (b < a) { // DRIFT. New fitness < old fitness
                    // do nothing.
                } else {
                    // Record the fitness increase in generations:
                    generations.push_back (geninfo(gen-lastgen, gen-lastf1, b));
                    lastgen = gen;
                    if (b==1.0) {
                        lastf1 = gen;
                        //LOG ("F=1 at generation " << gen);
                        ++f1count;
                    }

                    // Copy new fitness to ref
                    a = b;
                    // Copy new to reference
                    copy_genome (newg, refg);
                }
            }
        }

        LOG ("p=" << pOn << ". Generations size: " << generations.size() << " with " << f1count << " F=1 genomes found.");

        // Save data to file.
        ofstream f;
        stringstream pathss;
        pathss << "./";
        pathss << "evolve_";
        pathss << "a" << (unsigned int)target_ant << "_p" << (unsigned int)target_pos << "_";
        pathss << FF_NAME << "_" << nGenerations << "_gens_" << pOn << ".csv";

        f.open (pathss.str().c_str(), ios::out|ios::trunc);
        if (!f.is_open()) {
            cerr << "Error opening " << pathss.str() << endl;
            return 1;
        }

        for (unsigned int i = 0; i < generations.size(); ++i) {
            // In the file is recorded the time taken to get to F=1
            if (generations[i].fit == 1.0) {
                f << generations[i].gen_0 << endl;
            }
        }
        f.close();
    }

    return 0;
}
