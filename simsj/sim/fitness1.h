/*
 * This defines the fitness function created by Dan Whiteley
 */

#ifndef __FITNESS_FUNCTION__
#define __FITNESS_FUNCTION__

#include <math.h>

#define FF_NAME "ff1"

/*!
 * For the passed-in genome, find its final state, starting from the
 * anterior state state_ant and the posterior state state_pos. Return
 * a fitness specifier for the genome.
 *
 * This fitness specifier requires both networks to be point
 * attractors (ant and post). The fitness is then computed according
 * to:
 *
 * f = 1 - 0.1 (hamming_ant + hamming_pos)
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

    // state_ant and state_pos are the local copies of the gene state
    // each starting at initial_
    state_t state_ant = initial_ant;
    state_t state_pos = initial_pos;

    // The last state, used in the for loop.
    state_t state_ant_last = 0x0;
    state_t state_pos_last = 0x0;

    endpoint_t end_ant = ENDPOINT_UNKNOWN;
    endpoint_t end_pos = ENDPOINT_UNKNOWN;

    /*
     * Find out whether the anterior state results in the target.
     */
    DBGF ("ANTERIOR");

    set<state_t> visited_ant;
    visited_ant.insert (state_ant); // insert starting state

    int i_target_ant = -1;

    // Rather than while (!finished). May not be any better if breaking out...
    unsigned int i = 0;
    for (i = 0; i < 0xffffffff; ++i) {
        state_ant_last = state_ant;
        compute_next (genome, state_ant);

        if (state_ant == target_ant) {
            DBGF ("Anterior target (" << state_str(state_ant) << ") found on way to limit...");
            i_target_ant = i;
        }

        if (visited_ant.count (state_ant)) {
            // Already visited this state so it's a limit cycle
            DBGF ("Repeat state: " << state_str(state_ant) << "!");

            if (state_ant == target_ant) {
                DBGF ("Limit AND Target for " << state_str(state_ant) << "!");
                if (i_target_ant != -1) {
                    i_target_ant = i;
                }
            }

            if (state_ant == state_ant_last) {
                // It's a point attractor
                DBGF ("Point attractor");
                end_ant = ENDPOINT_POINT;
            } else {
                // It's a limit cycle.
                DBGF ("Limit cycle");
                end_ant = ENDPOINT_LIMIT;
            }
            break;
        }

        visited_ant.insert (state_ant);
    }

    /*
     * Note: We can stop here, with zero fitness, if the anterior
     * state ended up in a limit cycle.
     */

    state_t hamming_ant = compute_hamming (state_ant, target_ant);

    /*
     * Do the posterior state
     */
    DBGF ("POSTERIOR");

    set<state_t> visited_pos;
    visited_pos.insert (state_pos); // insert starting state

    int i_target_pos = -1;

    // Rather than while (!finished). May not be any better if breaking out...
    i = 0;
    for (i = 0; i < 0xffffffff; ++i) {
        state_pos_last = state_pos;
        compute_next (genome, state_pos);

        if (state_pos == target_pos) {
            DBGF ("Posterior target (" << state_str(state_pos) << ") found on way to limit...");
            i_target_pos = i;
        }

        if (visited_pos.count (state_pos)) {
            // Already visited this state; limit cycle...
            DBG2 ("Repeat state: " << state_pos << "!");

            if (state_pos == target_pos) {
                DBGF ("Limit AND Target for " << state_str(state_pos) << "!");
                if (i_target_pos != -1) {
                    i_target_pos = i;
                }
            }

            if (state_pos == state_pos_last) {
                // It's a point attractor
                DBGF ("Point attractor");
                end_pos = ENDPOINT_POINT;
            } else {
                // It's a limit cycle.
                DBGF ("Limit cycle");
                end_pos = ENDPOINT_LIMIT;
            }
            break;
        }

        visited_pos.insert (state_pos);
    }

    state_t hamming_pos = compute_hamming (state_pos, target_pos);

    DBGF ("Anterior ended on " << (end_ant == ENDPOINT_POINT ? "point attractor" : ( end_ant == ENDPOINT_LIMIT ? "limit cycle" : "unknown") ));
    DBGF ("Iterations from anterior start point to target: " << i_target_ant);
    DBGF ("Posterior ended on " << (end_pos == ENDPOINT_POINT ? "point attractor" : ( end_pos == ENDPOINT_LIMIT ? "limit cycle" : "unknown") ));
    DBGF ("Iterations from posterior start point to target: " << i_target_pos);
    DBGF ("hamming_ant: " << (unsigned int)hamming_ant << ", hamming_pos: " << (unsigned int)hamming_pos);

    if (end_ant == ENDPOINT_POINT && end_pos == ENDPOINT_POINT) {
        fitness = 1.0f - (0.1)* (hamming_ant+hamming_pos); // 0.1 == 1.0f/(2*N_Genes) - the max Hamming distance possible
    } else {
        fitness = 0.0f;
    }

    DBGF ("Fitness: " << fitness);
    return fitness;
}
#endif // __FITNESS_FUNCTION__
