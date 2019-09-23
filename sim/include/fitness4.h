/*
 * This defines a fitness function described in the associated
 * paper. See the code comment for evaluate_fitness() for a
 * description of the fitness function.
 *
 * This is the "multiplicative fitness function"
 */

#ifndef __FITNESS_FUNCTION__
#define __FITNESS_FUNCTION__

#include <set>
#include <array>

using namespace std;

/*!
 * When working with states in a graph of nodes, it may be necessary
 * to use one bit to refer to the state as being unset; this is the
 * bit to use.
 */
#define state_t_unset 0x80

#define FF_NAME "ff4"

double
evaluate_one_async (array<genosect_t, N_Genes>& genome, state_t state, state_t target)
{
    double score = 0.0;
    unsigned int twoN = N_Genes * (1<<N_Genes);

    array<double, N_Genes> sc;
    for (unsigned int j = 0; j < N_Genes; ++j) {
        sc[j] = 0.0;
    }

    // Just "develop" the state 2N times and add up the total of the scores. Simpler than the sync
    // case.
    for (unsigned int t=0; t<twoN; ++t) {

        compute_next_async (genome, state);

        state_t a = (state ^ ~target) & state_mask;
        for (unsigned int j = 0; j < N_Genes; ++j) {
            sc[j] += static_cast<double>( (a >> j) & 0x1 );
        }
    }

    score = pow(static_cast<double>(twoN), -N_Genes);
    for (unsigned int j = 0; j < N_Genes; ++j) {
        score *= sc[j];
    }

    return score;
}

/*!
 * Evaluates the fitness of one context (anterior or posterior in the 2-context system).
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

            // Already visited this state so it's a limit cycle or point attractor

            if (state == state_last) { // Point attractor

                score = (state == target) ? 1.0 : score; // score is 0

            } else { // Limit cycle

                // Determine the states in the limit cycle by going around it once more.
                set<state_t> lc;
                unsigned int lc_len = 0;
                while (lc.count (state) == 0) {
                    // Check if we have one or both target states on this limit cycle
                    lc.insert (state);
                    lc_len++;
                    // DBGF ("Limit cycle contains: " << state_str (state));
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

                score = pow(static_cast<double>(lc_len), -N_Genes);
                for (unsigned int j = 0; j < N_Genes; ++j) {
                    score *= sc[j];
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

#ifdef DEBUGF
    if (fitness == 1.0) {
        LOG ("F=1 genome found.");
        show_genome (genome);
    }
    DBGF ("Fitness: " << fitness);
    cout << genome2str(genome) << ", fitness: " << fitness << endl;
#endif

    return fitness;
}

/*
 * A version of evaluate_fitness which takes vectors of initial and target states and computes a
 * fitness score.
 */
double
evaluate_fitness (array<genosect_t, N_Genes>& genome,
                  vector<state_t>& initials, vector<state_t>& targets, const bool& async_devel)
{
    if (initials.size() != targets.size()) {
        throw runtime_error ("initials vector is a different length from the targets vector");
    }
    double fitness = 1.0;
    if (async_devel) {
        for (unsigned int i = 0; i < initials.size(); ++i) {
            double score = evaluate_one_async (genome, initials[i], targets[i]);
            fitness *= score;
        }
    } else {
        for (unsigned int i = 0; i < initials.size(); ++i) {
            double score = evaluate_one (genome, initials[i], targets[i]);
            fitness *= score;
        }
    }
    return fitness;
}

#endif // __FITNESS_FUNCTION__
