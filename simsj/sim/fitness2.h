/*
 * This defines a fitness function described in the associated
 * paper. See the code comment for evaluate_fitness() for a
 * description of the fitness function.
 */

#ifndef __FITNESS_FUNCTION__
#define __FITNESS_FUNCTION__

#include <set>

using namespace std;

/*!
 * When working with states in a graph of nodes, it may be necessary
 * to use one bit to refer to the state as being unset; this is the
 * bit to use.
 */
#define state_t_unset 0x80

#define FF_NAME "ff2"

/*!
 * For the passed-in genome, find its final state, starting from the
 * anterior state initial_ant and the posterior state initial_pos
 * (stored in global variables). Return a fitness specifier for the
 * genome.
 *
 * This function requires the targets to be situated ON a limit cycle
 * (or ON a fixed point attractor). Both targets could be on the same
 * attractor network, but only if both inits are also on that
 * attractor network and both targets are on the same limit cycle.
 *
 * The fitness is then computed according
 * to:
 *
 * f = 1 - S/(2 * 2^N_Genes)
 *
 * Where S is: the number of states in the anterior limit cycle - 1
 * plus the number of states in the posterior limit cycle - 1.
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

    // What kind of limit does the network starting with the initial
    // anterior state reach? Also, the same information for the
    // posterior initial state.
    //endpoint_t end_ant = ENDPOINT_UNKNOWN;
    //endpoint_t end_pos = ENDPOINT_UNKNOWN;

    // For computing the distance from the target state to the limit
    // of the basin of attraction
#ifdef TARGET_NOT_REQD_TO_BE_EXACTLY_AT_POINT_ATTRACTOR
    int i_target_ant = -1;
    int i_target_pos = -1;
#endif
    int dist_to_ant = -1;
    int dist_to_pos = -1;

#ifdef TARGET_NOT_REQD_TO_BE_EXACTLY_AT_POINT_ATTRACTOR
    // Was the posterior target found when starting from the anterior
    // initial state?
    bool targ_pos_from_ant_init = false;
    // Vice versa: Was the ANterior target found when starting from
    // the POSterior initial state?
    bool targ_ant_from_pos_init = false;
#endif

    // To record the actual state reached at a fixed point attractor
    //state_t end_ant_state = state_t_unset;
    //state_t end_pos_state = state_t_unset;

    // Flag to mark if the target anterior state is found on a limit
    // cycle reached from the anterior initial state.
    bool targ_ant_on_ant_limit = false;
    // Flag to mark if the target posterior state is found on a limit
    // cycle reached from the anterior initial state.
    //bool targ_pos_on_ant_limit = false;
    // Flag to mark if the target anterior state is found on a limit
    // cycle reached from the posterior initial state.
    //bool targ_ant_on_pos_limit = false;
    // Flag to mark if the target posterior state is found on a limit
    // cycle reached from the posterior initial state.
    bool targ_pos_on_pos_limit = false;

    bool finished = false;
    int i = 0;

    /*
     * Find out whether the anterior state results in the target.
     */
    DBGF ("ANTERIOR");

    set<state_t> visited_ant;
    visited_ant.insert (state_ant); // insert starting state

    finished = false;
    i = 0;
    while (!finished) {
        state_ant_last = state_ant;
        compute_next (genome, state_ant);
        ++i;

#ifdef TARGET_NOT_REQD_TO_BE_EXACTLY_AT_POINT_ATTRACTOR
        if (state_ant == target_ant) {
            DBGF ("Anterior target (" << state_str(state_ant) << ") found on way to limit...");
            i_target_ant = i;
        } else if (state_ant == target_pos) {
            // Found posterior target on this attractor network. NB:
            // This won't guarantee that we find posterior target on
            // the WHOLE anterior basin of attraction, if the
            // posterior target is upstream from the anterior init.
            targ_pos_from_ant_init = true;
        }
#endif

        if (visited_ant.count (state_ant)) {
            // Already visited this state so it's a limit cycle
            DBGF ("Repeat state: " << state_str(state_ant)
                  << "! (last state: " << state_str(state_ant_last) << ")");

            if (state_ant == state_ant_last) {

                DBGF ("Point attractor");
                //end_ant = ENDPOINT_POINT;
                //end_ant_state = state_ant;

                if (state_ant == target_ant) {
                    targ_ant_on_ant_limit = true;
                    dist_to_ant = 0;
#ifdef TARGET_NOT_REQD_TO_BE_EXACTLY_AT_POINT_ATTRACTOR
                } else if (i_target_ant != -1) {
                    dist_to_ant = i - i_target_ant;
#endif
                } // else dist_to_ant is still -1

#ifdef TARGET_NOT_REQD_TO_BE_EXACTLY_AT_POINT_ATTRACTOR
                // Then there is more complication. I think we have to
                // be able to map out the whole basin in this case.
                //
                // Even if we have a point attractor with the anterior
                // target on the basin of attraction, if the point
                if (targ_pos_from_ant_init == true) {
                    dist_to_ant = -1;
                }
#endif
                DBGF("Distance from target to point attractor: " << dist_to_ant);

            } else {

                DBGF ("Limit cycle");
                //end_ant = ENDPOINT_LIMIT;

                // Determine size of limit cycle (stored in
                // dist_to_ant) by going around it once more
                set<state_t> visited;
                dist_to_ant = 0;
                while (visited.count (state_ant) == 0) {
                    // Check if we have one or both target states on
                    // this limit cycle
                    if (state_ant == target_ant) {
                        targ_ant_on_ant_limit = true;
                        DBGF ("Ant LC contains ant target");
                    }
                    if (state_ant == target_pos) {
                        //targ_pos_on_ant_limit = true;
                        DBGF ("Ant LC contains pos target");
                        // maybe: dist_to_ant--;
                    }
                    visited.insert (state_ant);
                    dist_to_ant++;
                    DBGF ("Limit cycle contains: " << state_str (state_ant));
                    compute_next (genome, state_ant);
                }
                // Have counted the size of the limit cycle in
                // dist_to_ant. Now see if we need to reset it back to
                // -1
                if (targ_ant_on_ant_limit == false) {
                    dist_to_ant = -1;
                }

                DBGF ("Limit cycle size: " << dist_to_ant);
            }
            break;
        }

        visited_ant.insert (state_ant);
    }

    /*
     * Do the posterior state
     */
    DBGF ("POSTERIOR");

    set<state_t> visited_pos;
    visited_pos.insert (state_pos); // insert starting state

    finished = false;
    i = 0;
    while (!finished) {
        state_pos_last = state_pos;
        compute_next (genome, state_pos);
        ++i;

#ifdef TARGET_NOT_REQD_TO_BE_EXACTLY_AT_POINT_ATTRACTOR
        if (state_pos == target_pos) {
            DBGF ("Posterior target (" << state_str(state_pos) << ") found on way to limit...");
            i_target_pos = i;
        } else if (state_pos == target_pos) {
            // Found posterior target on this attractor network. NB:
            // This won't guarantee that we find posterior target on
            // the WHOLE posterior basin of attraction, if the
            // posterior target is upstream from the posterior init.
            targ_pos_from_pos_init = true;
        }
#endif

        if (visited_pos.count (state_pos)) {
            // Already visited this state so it's a limit cycle
            DBGF ("Repeat state: " << state_str(state_pos)
                  << "! (last state: " << state_str(state_pos_last) << ")");

            if (state_pos == state_pos_last) {

                DBGF ("Point attractor");
                //end_pos = ENDPOINT_POINT;
                //end_pos_state = state_pos;

                if (state_pos == target_pos) {
                    targ_pos_on_pos_limit = true;
                    dist_to_pos = 0;
#ifdef TARGET_NOT_REQD_TO_BE_EXACTLY_AT_POINT_ATTRACTOR
                } else if (i_target_pos != -1) {
                    dist_to_pos = i - i_target_pos;
#endif
                } // else dist_to_pos is still -1

#ifdef TARGET_NOT_REQD_TO_BE_EXACTLY_AT_POINT_ATTRACTOR
                // Then there is more complication. I think we have to
                // be able to map out the whole basin in this case.
                //
                // Even if we have a point attractor with the posterior
                // target on the basin of attraction, if the point
                if (targ_pos_from_pos_init == true) {
                    dist_to_pos = -1;
                }
#endif

                DBGF("Distance from target to point attractor: " << dist_to_pos);

            } else {

                DBGF ("Limit cycle");
                //end_pos = ENDPOINT_LIMIT;

                // Determine size of limit cycle (stored in
                // dist_to_pos) by going around it once more
                set<state_t> visited;
                dist_to_pos = 0;
                while (visited.count (state_pos) == 0) {
                    // Check if we have one or both target states on
                    // this limit cycle
                    if (state_pos == target_pos) {
                        targ_pos_on_pos_limit = true;
                        DBGF ("Pos LC contains pos target");
                    }
                    if (state_pos == target_ant) {
                        //targ_ant_on_pos_limit = true;
                        DBGF ("Pos LC contains ant target");
                        // maybe: dist_to_pos--;
                    }
                    visited.insert (state_pos);
                    dist_to_pos++;
                    DBGF ("Limit cycle contains: " << state_str (state_pos));
                    compute_next (genome, state_pos);
                }
                // Have counted the size of the limit cycle in
                // dist_to_pos. Now see if we need to reset it back to
                // -1
                if (targ_pos_on_pos_limit == false) {
                    dist_to_ant = -1;
                }

                DBGF ("Limit cycle size: " << dist_to_pos);
            }
            break;
        }
        visited_pos.insert (state_pos);
    }

    // Finally, compute fitness.
    if (dist_to_ant != -1 && dist_to_pos != -1) {
        fitness = 1.0f - (dist_to_ant + dist_to_pos)/static_cast<float>(1<<(N_Genes+1));
    }

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
