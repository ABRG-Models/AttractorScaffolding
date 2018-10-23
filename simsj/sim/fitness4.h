/*
 * This defines a fitness function described in the associated
 * paper. See the code comment for evaluate_fitness() for a
 * description of the fitness function.
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

float
evaluate_one (array<genosect_t, N_Genes>& genome, state_t state, state_t target)
{
    float score = 0.0f;

    state_t state_last = state_t_unset;
    set<state_t> visited;
    visited.insert (state); // insert starting state
    for (;;) {
        state_last = state;
        compute_next (genome, state);

        if (visited.count (state)) {
            // Already visited this state so it's a limit cycle
            DBGF ("Repeat state: " << state_str(state)
                  << "! (last state: " << state_str(state_last) << ")");

            if (state == state_last) {
                DBGF ("Point attractor");
                if (state == target) {
                    score = 1.0f;
                } // else score is definitely 0.

            } else {
                DBGF ("Limit cycle");

                // Determine the states in the limit cycle by going
                // around it once more.
                set<state_t> lc;
                unsigned int lc_len = 0;
                while (lc.count (state) == 0) {
                    // Check if we have one or both target states on
                    // this limit cycle
                    lc.insert (state);
                    lc_len++;
                    DBGF ("Limit cycle contains: " << state_str (state));
                    compute_next (genome, state);
                }

                // Now have the set lc; can work out its score.

                // For tabulating the scores
                array<float, N_Genes> sc;
                for (unsigned int j = 0; j < N_Genes; ++j) { sc[j] = 0.0f; }

                set<state_t>::const_iterator i = lc.begin();
                while (i != lc.end()) {
                    state_t a = (*i) ^ target;
                    for (unsigned int j = 0; j < N_Genes; ++j) {
                        sc[j] += static_cast<float>((a & (1<<j)) >> j)/static_cast<float>(lc_len);
                    }
                    ++i;
                }
                DBGF("Score:");
#ifdef DEBUGF
                cout << sc[0];
#endif
                score = sc[0];
                for (unsigned int j = 1; j < N_Genes; ++j) {
#ifdef DEBUGF
                    cout << "," << sc[j];
#endif
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
 * Returns fitness in range 0 to 1.0
 *
 * For further details on this fitness evaluation, please see the
 * associated paper.
 */
float
evaluate_fitness (array<genosect_t, N_Genes>& genome)
{
    float fitness = 0.0f;

    float ant_score = evaluate_one (genome, initial_ant, target_ant);
    float pos_score = evaluate_one (genome, initial_pos, target_pos);
    fitness = ant_score * pos_score;

#ifdef DEBUG
    if (fitness == 1.0f) {
        LOG ("F=1 genome found.");
        show_genome (genome);
    }
#endif

    DBGF ("Fitness: " << fitness);
    return fitness;
}

#endif // __FITNESS_FUNCTION__
