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

/*!
 * When working with states in a graph of nodes, it may be necessary
 * to use one bit to refer to the state as being unset; this is the
 * bit to use.
 */
#define state_t_unset 0x80

#define FF_NAME "ff0"

/*!
 * To make a graph of states, we need a state node object which has
 * one child node to which it transfers, but potentially many parent
 * nodes. If parents set is empty, then this StateNode is a starting
 * state in a basin of attraction.
 */
class StateNode
{
public:
    StateNode(state_t s)
        : id(s) { }
    /*!
     * The identifier of this base node - its value. This is used as
     * the key in maps of StateNodes.
     */
    state_t id;
    /*!
     * The parents of the base node, which feed into it.
     */
    set<state_t> parents;
    /*!
     * the child StateNode
     */
    state_t child;
};

#define CONTAINS_TARGET_ANT  0x1
#define CONTAINS_TARGET_POS  0x2
#define CONTAINS_INITIAL_ANT 0x4
#define CONTAINS_INITIAL_POS 0x8

/*!
 * Need a container to hold the information about a basin of attraction.
 */
class BasinOfAttraction
{
public:
    /*!
     * Merge the other basin of attraction into this basin of
     * attraction. The assumption is that the limitCycle and endpoint
     * of other match the limitCycle and endpoint of this (I'm not
     * going to waste compute cycles comparing, as this will already
     * have been done).
     */
    void merge (const BasinOfAttraction& other) {
        this->flags |= other.flags;
        // merge other.nodes into this->nodes
        map<state_t, StateNode>::iterator mi = this->nodes.begin();
        // Add parents from other states to parents of this
        while (mi != this->nodes.end()) {
            state_t key = mi->first;
            if (other.nodes.count(key)) {
                mi->second.parents.insert (other.nodes.at(key).parents.begin(),
                                           other.nodes.at(key).parents.end());
            }
            ++mi;
        }
        // THEN add any states in other.nodes that don't exist in
        // this.
        map<state_t, StateNode>::const_iterator cmi = other.nodes.begin();
        while (cmi != other.nodes.end()) {
            // cmi->first is the state_t id of a node in the other
            // basin of attraction.
            if (this->nodes.count (cmi->first) == 0) {
                this->nodes.insert (make_pair(cmi->first, cmi->second));
            }
            ++cmi;
        }
        DBGF ("After merge, this->nodes.size() = " << this->nodes.size());
    }

    /*!
     * An "output for debugging" method
     */
    void debug (BasinOfAttraction& bi) {
        cout << "----------------------Basin-output-begin---------------------------" << endl;
        cout << "Basin of attraction with the attractor:" << endl;
        set<state_t>::iterator si = bi.limitCycle.begin();
        while (si != bi.limitCycle.end()) {
            cout << "  " << state_str (*si) << endl;
            si++;
        }
        cout << "Branches:" << endl;
        map<state_t, StateNode>::const_iterator mi = bi.nodes.begin();
        while (mi != bi.nodes.end()) {
            if (mi->second.parents.empty()) {
                // Then this is an "outer node" on the basin. Show
                // its progress to the attractor
                state_t state = mi->first;
                StateNode sn = mi->second;
                while (bi.limitCycle.count(state) == 0) {
                    cout << " --> " << state_str (state);
                    state = sn.child;
                    sn = bi.nodes.at (state);
                }
                set<state_t>::const_iterator si = bi.limitCycle.begin();
                cout << " -->* ";
                while (si != bi.limitCycle.end()) {
                    cout << state_str (*si) << ":";
                    ++si;
                }
                cout << endl;

            }
            ++mi;
        }
        cout << "Contains: ant initial,  ant target, pos initial,  pos target?" << endl;
        cout << "          ";
        cout << (bi.flags & CONTAINS_INITIAL_ANT ? "Yes          ":"No           ");
        cout << (bi.flags & CONTAINS_TARGET_ANT ?  "Yes          ":"No           ");
        cout << (bi.flags & CONTAINS_INITIAL_POS ? "Yes          ":"No           ");
        cout << (bi.flags & CONTAINS_TARGET_POS ?  "Yes          ":"No           ") << endl;
        if ((bi.flags & (CONTAINS_TARGET_ANT | CONTAINS_INITIAL_ANT)) == (CONTAINS_TARGET_ANT | CONTAINS_INITIAL_ANT)
            && (bi.flags & (CONTAINS_TARGET_ANT | CONTAINS_INITIAL_ANT)) != 0) {
            cout << "Distance from ant target to attractor: " << bi.dist_to_ant << endl;
        }
        if ((bi.flags & (CONTAINS_TARGET_POS | CONTAINS_INITIAL_POS)) == (CONTAINS_TARGET_POS | CONTAINS_INITIAL_POS)
            && (bi.flags & (CONTAINS_TARGET_POS | CONTAINS_INITIAL_POS)) != 0) {
            cout << "Distance from pos target to attractor: " << bi.dist_to_pos << endl;
        }
        cout << "Nodes in basin:" << bi.nodes.size() << endl;
        cout << "-----------------------Basin-output-end----------------------------" << endl;
    }

    /*!
     * flags in which to mark if the anterior target is in this basin,
     * and whether the posterior target is in the basin.
     */
    unsigned int flags = 0x0;

    /*!
     * Distance to anterior and posterior targets. -1 means unset.
     */
    //@{
    int dist_to_ant = -1;
    int dist_to_pos = -1;
    //@}

    /*!
     * Is the endpoint type a fixed attractor or a limit cycle?
     */
    endpoint_t endpoint = ENDPOINT_UNKNOWN;

    /*!
     * The set of states in the limit cycle. Will be a set of size 1
     * if endpoint is a fixed point attractor. This is used to
     * determine if one partially determined basin of attraction
     * matches another, determined basin of attraction.
     */
    set<state_t> limitCycle;

    /*!
     * The full basin of attraction.
     */
    map<state_t, StateNode> nodes;
};

/*!
 * Find all the basins of attraction for the given genome.
 *
 * Return -1 if the function exits early because it has detected that
 * fitness will be zero.
 */
int
find_basins_of_attraction (array<genosect_t, N_Genes>& genome,
                           vector<BasinOfAttraction>& basins,
                           bool exit_early_if_possible = true)
{
    int rtn = 0;

    for (state_t s = 0; s < (1<<N_Genes); ++s) {

        // First check if s is in any of the basins we already computed.
        bool s_encountered = false;
        vector<BasinOfAttraction>::iterator bi = basins.begin();
        while (bi != basins.end()) {
            if (bi->nodes.count(s) > 0) {
                s_encountered = true;
                break;
            }
            ++bi;
        }
        if (s_encountered == true) {
            continue; // on to next s
        }

        // A new basin of attraction that we'll find.
        BasinOfAttraction basin;
        // A copy of starting point s.
        state_t st = s;
        state_t next_st = state_t_unset;
        state_t last_st = state_t_unset;

        unsigned int saw_flags = 0x0;

        for (;;) {
            // For the current state, st compute what the next state will be.
            next_st = st;
            compute_next (genome, next_st);

            // Check that BOTH targets aren't simultaneously present
            // in this sub-section of the basin of attraction.
            if (st == target_ant) {
                saw_flags |= CONTAINS_TARGET_ANT;
                if (saw_flags & CONTAINS_TARGET_POS) {
                    if (exit_early_if_possible) {
                        return -1;
                    }
                }
            } else if (st == target_pos) {
                saw_flags |= CONTAINS_TARGET_POS;
                if (saw_flags & CONTAINS_TARGET_ANT) {
                    if (exit_early_if_possible) {
                        return -1;
                    }
                }
            }

            // Create state node
            StateNode stnode(st); // node for current state.
            stnode.child = next_st; // Mark the child state
            if (last_st != state_t_unset) {
                stnode.parents.insert (last_st);
            }

            if (basin.nodes.count (st) > 0) {
                // Already visited this state so it's in an attractor
                DBG2 ("Repeat st " << state_str(st) << "!");
                if (st == last_st) {
                    // It's a point attractor
                    DBGF("fixed point attractor");
                    basin.endpoint = ENDPOINT_POINT;
                    // Which means it's its own parent. Set parent of the node that's ALREADY BEEN added to basin.
                    basin.nodes.find(st)->second.parents.insert (st);
                    basin.limitCycle.insert (st);
                } else {
                    // It's a limit cycle.
                    DBGF("limit cycle attractor");
                    // Go around the limit cycle to determine its identity, which is a set of states.
                    basin.endpoint = ENDPOINT_LIMIT;
                    // Update the parent of the copy of this state that's already in the basin object
                    basin.nodes.find(st)->second.parents.insert (last_st);

                    // Cycle around filling the limit cycle set in basin.limitCycle
                    state_t lc = st;
                    for (;;) {
                        basin.limitCycle.insert (lc);
                        state_t c = basin.nodes.find (lc)->second.child;
                        if (c == st) {
                            // We're back to the start
                            break;
                        } else {
                            lc = c;
                        }
                    }
                }

                break;
            }

            // Insert it into basin
            basin.nodes.insert (make_pair (st, stnode));
            DBG2 ("Inserting " << state_str (st));

            // Update last_st and st
            last_st = st;
            st = next_st;
        }

        // Now see if our basin is already present in basins, and if not, simply push_back.
        bool found = false;
        bi = basins.begin();
        while (bi != basins.end()) {
            // Nice thing with sets is that we can directly compare them.
            if (basin.limitCycle == bi->limitCycle) {
                // Merge basin into bi->second, because this limitCycle was already found.
                bi->merge (basin);
                found = true;
                break;
            }
            ++bi;
        }
        if (!found) {
            basins.push_back (basin);
        }
    }

    return rtn;
}

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
    int brtn = find_basins_of_attraction (genome, basins, true); // true to exit early from finding basins if F will be 0
    if (brtn == -1) {
        // In find_basins_of_attraction it was determined that the fitness will be 0.
        return fitness;
    }

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
            bi->debug (*bi);
            ++bi;
        }
        LOG ("======================================================");
    }
#endif

    DBGF ("Fitness: " << fitness);
    return fitness;
}

#endif // __FITNESS_FUNCTION__
