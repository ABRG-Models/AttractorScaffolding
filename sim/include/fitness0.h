/*
 * This defines Stuart Wilson's original fitness function described in
 * the associated paper.
 */

#ifndef __FITNESS_FUNCTION__
#define __FITNESS_FUNCTION__

#include <math.h>
#include <set>
#include <map>
#include <vector>

using namespace std;

#define FF_NAME "ff0"

#include "basins.h"

/*!
 * For the passed-in genome, find its final state, starting from the
 * anterior state initial_ant and the posterior state
 * initial_pos. Return a fitness specifier for the genome.
 *
 * This fitness specifier requires anterior and posterior starting
 * states to be within different basins of attraction, each ending in
 * either a fixed point attractor or a limit cycle. The fitness is
 * then computed according to:
 *
 * f = 1 - S/(2 * 2^N_Genes)
 *
 * Where S is the number of steps it takes to go from the anterior
 * target state to its attractor plus the number of steps it takes to
 * go from the posterior target state to its attractor IF the
 * attractor also includes the initial_pos state (and not the
 * initial_ant state).
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

    // The first job is to map out all the basins of attraction that
    // exist for the given genome, starting from every possible start
    // point.
    vector<BasinOfAttraction> basins;
    find_basins_of_attraction (genome, basins);

    // Now we have the basins, check through them to see which contain
    // the target states, and if so, work out the distance from the
    // target state to the attractor. If the target is IN the limit
    // cycle, then the distance is the size of the limit cycle.
    vector<BasinOfAttraction>::iterator bi = basins.begin();
    bi = basins.begin();
    bool separate_attractors = true;
    vector<BasinOfAttraction>::iterator ant_basin = basins.end();
    vector<BasinOfAttraction>::iterator pos_basin = basins.end();
    while (bi != basins.end()) {

        if (bi->nodes.count (target_ant)) {

            // This basin of attraction contains the anterior target state
            bi->flags |= CONTAINS_TARGET_ANT;
            if (bi->nodes.count (initial_ant)) {
                bi->flags |= CONTAINS_INITIAL_ANT;
                ant_basin = bi;
            } else {
                // fitness will be 0 because this basin of attraction
                // contains only TARGET_ANT and not INITIAL_ANT.
                break;
            }

            if (bi->limitCycle.count (target_ant)) {
                // ant target is in limit cycle.
                bi->dist_to_ant = bi->limitCycle.size()-1;
            } else {
                // Step from ant target to the limit cycle
                state_t c = bi->nodes.at(target_ant).child;
                bi->dist_to_ant = 1;
                while (bi->limitCycle.count (c) == 0) {
                    c = bi->nodes.at(c).child;
                    bi->dist_to_ant++;
                }
                bi->dist_to_ant += bi->limitCycle.size()-1;
            }
            DBGF ("Dist from ant target to ant attractor is " << bi->dist_to_ant << " or " << ant_basin->dist_to_ant);
        }

        if (bi->nodes.count (target_pos)) {
            bi->flags |= CONTAINS_TARGET_POS;
            if (bi->nodes.count (initial_pos)) {
                bi->flags |= CONTAINS_INITIAL_POS;
                pos_basin = bi;
            } else {
                // fitness will be 0 because this basin of attraction
                // contains only TARGET_POS and not INITIAL_POS.
                break;
            }
            // Check if we have targ_ant in this basin of attraction
            // as well; if so set separate_attractors = false;
            if (bi->flags & CONTAINS_TARGET_ANT) {
                separate_attractors = false;
            }

            if (bi->limitCycle.count (target_pos)) {
                // pos target is in limit cycle.
                bi->dist_to_pos = bi->limitCycle.size()-1;
            } else {
                // Step from pos target to the limit cycle
                state_t c = bi->nodes.at(target_pos).child;
                bi->dist_to_pos = 1;
                while (bi->limitCycle.count (c) == 0) {
                    c = bi->nodes.at(c).child;
                    bi->dist_to_pos++;
                }
                bi->dist_to_pos += bi->limitCycle.size()-1;
            }
            DBGF ("Dist from pos target to pos attractor is " << bi->dist_to_pos << " or " << pos_basin->dist_to_pos);
        }
        ++bi;
    }

    // Finally, compute fitness.
    if (separate_attractors == true
        && ant_basin != basins.end()
        && pos_basin != basins.end()) {
        fitness = 1.0f - (ant_basin->dist_to_ant + pos_basin->dist_to_pos)/static_cast<float>(1<<(N_Genes+1));
    } // else fitness is 0.

#ifdef DEBUG
    if (fitness == 1.0f) {
        LOG ("======================================================");
        LOG ("F=1 genome found.");
        show_genome (genome);
        LOG ("Basins of attraction for this F=1 genome:");
        bi = basins.begin();
        int bcount_ = 0;
        while (bi != basins.end()) {
            LOG ("Basin " << bcount_++);
            bi->debug();
            ++bi;
        }
        LOG ("======================================================");
    }
#endif

    DBGF ("Fitness: " << fitness);
    return fitness;
}

#endif // __FITNESS_FUNCTION__
