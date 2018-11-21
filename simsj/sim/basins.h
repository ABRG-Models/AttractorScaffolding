/*
 * Basins of attraction code.
 */

#ifndef __BASINS_H__
#define __BASINS_H__

#include <map>
#include <set>
#include <vector>

#include "endpoint.h"

using namespace std;

/*!
 * When working with states in a graph of nodes, it may be necessary
 * to use one bit to refer to the state as being unset; this is the
 * bit to use.
 */
#define state_t_unset 0x80

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
    void debug (void) const {
        cout << "----------------------Basin-output-begin---------------------------" << endl;
        cout << "Basin of attraction with the attractor:" << endl;
        set<state_t>::const_iterator si = this->limitCycle.begin();
        while (si != this->limitCycle.end()) {
            cout << "  " << state_str (*si) << endl;
            si++;
        }
        cout << "Branches:" << endl;
        map<state_t, StateNode>::const_iterator mi = this->nodes.begin();
        while (mi != this->nodes.end()) {
            if (mi->second.parents.empty()) {
                // Then this is an "outer node" on the basin. Show
                // its progress to the attractor
                state_t state = mi->first;
                StateNode sn = mi->second;
                while (this->limitCycle.count(state) == 0) {
                    cout << " --> " << state_str (state);
                    state = sn.child;
                    sn = this->nodes.at (state);
                }
                set<state_t>::const_iterator si = this->limitCycle.begin();
                cout << " -->* ";
                while (si != this->limitCycle.end()) {
                    cout << state_str (*si) << ":";
                    ++si;
                }
                cout << endl;

            }
            ++mi;
        }
        cout << "Contains: ant initial,  ant target, pos initial,  pos target?" << endl;
        cout << "          ";
        cout << (this->flags & CONTAINS_INITIAL_ANT ? "Yes          ":"No           ");
        cout << (this->flags & CONTAINS_TARGET_ANT ?  "Yes          ":"No           ");
        cout << (this->flags & CONTAINS_INITIAL_POS ? "Yes          ":"No           ");
        cout << (this->flags & CONTAINS_TARGET_POS ?  "Yes          ":"No           ") << endl;
        if ((this->flags & (CONTAINS_TARGET_ANT | CONTAINS_INITIAL_ANT)) == (CONTAINS_TARGET_ANT | CONTAINS_INITIAL_ANT)
            && (this->flags & (CONTAINS_TARGET_ANT | CONTAINS_INITIAL_ANT)) != 0) {
            cout << "Distance from ant target to attractor: " << this->dist_to_ant << endl;
        }
        if ((this->flags & (CONTAINS_TARGET_POS | CONTAINS_INITIAL_POS)) == (CONTAINS_TARGET_POS | CONTAINS_INITIAL_POS)
            && (this->flags & (CONTAINS_TARGET_POS | CONTAINS_INITIAL_POS)) != 0) {
            cout << "Distance from pos target to attractor: " << this->dist_to_pos << endl;
        }
        cout << "Nodes in basin:" << this->nodes.size() << endl;
        cout << "Transitions in basin:" << endl;
        mi = this->nodes.begin();
        while (mi != this->nodes.end()) {
            cout << state_str(mi->second.id) << " --> " << state_str(mi->second.child) << endl;
            ++mi;
        }
        cout << "-----------------------Basin-output-end----------------------------" << endl;
    }

    /*!
     * Return the set of state to state transitions in this basin of attraction.
     */
    set<unsigned int> getTransitionSet (void) const {
        set<unsigned int> transitions;
         map<state_t, StateNode>::const_iterator mi = this->nodes.begin();
        while (mi != this->nodes.end()) {
            unsigned int trans = ((unsigned int)mi->second.id << 16) | (unsigned int)mi->second.child;
            DBG2 ("Inserting transition 0x" << hex << trans << dec);
            transitions.insert (trans);
            ++mi;
        }
        return transitions;
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
 */
void
find_basins_of_attraction (array<genosect_t, N_Genes>& genome,
                           vector<BasinOfAttraction>& basins)
{
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
            // For the current state, st, compute what the next state will be.
            next_st = st;
            compute_next (genome, next_st);

            // Check that BOTH targets aren't simultaneously present
            // in this sub-section of the basin of attraction.
            if (st == target_ant) {
                saw_flags |= CONTAINS_TARGET_ANT;
            } else if (st == target_pos) {
                saw_flags |= CONTAINS_TARGET_POS;
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
}

/*!
 * Container class to hold several basins of attraction for a
 * particular genome and some information that can be obtained from
 * them.
 */
class AllBasins
{
public:
    AllBasins() {}
    AllBasins (const array<genosect_t, N_Genes>& g) {
        this->update (g);
    }
    ~AllBasins() {}

    void update (const array<genosect_t, N_Genes>& g) {
        this->genome = g;
        this->basins.clear();
        this->attractorSizes.clear();
        this->transitions.clear();
        find_basins_of_attraction (this->genome, this->basins);
        vector<BasinOfAttraction>::const_iterator i = this->basins.begin();
        while (i != basins.end()) {
            set<unsigned int> tset = i->getTransitionSet();
            this->transitions.insert(tset.begin(), tset.end());
            this->attractorSizes.push_back(i->limitCycle.size());
            ++i;
        }
    }

    //! The genome for which this was determined
    array<genosect_t, N_Genes> genome;

    //! All the basins of attraction.
    vector<BasinOfAttraction> basins;

    unsigned int getNumBasins (void) {
        return this->basins.size();
    }

    /*!
     * Holds a list of the sizes of the attractor limit cycles.
     */
    vector<unsigned int> attractorSizes;

    double meanAttractorLength (void) {
        unsigned int sum = 0.0;
        for (unsigned int i : this->attractorSizes) {
            sum += i;
        }
        return (static_cast<double>(sum)/static_cast<double>(attractorSizes.size()));
    }

    unsigned int maxAttractorLength (void) {
        unsigned int max = 0;
        for (unsigned int i : this->attractorSizes) {
            max = i > max ? i : max;
        }
        return max;
    }

    /*!
     * Holds all the transitions in all the basins. Should have size
     * exactly 2^N_Genes. Will break for N_Genes > 16.
     */
    set<unsigned int> transitions;
};

/*!
 * A class to hold information about one network and its comparison
 * with any other networks.
 */
class NetInfo
{
public:
    NetInfo() {}
    NetInfo(AllBasins& ab_, unsigned int gen, double fitn) {
        this->update (ab_, gen, fitn);
    }
    void update (AllBasins& ab_, unsigned int gen, double fitn) {
        this->ab = ab_;
        this->generation = gen;
        this->fitness = fitn;
    }
    ~NetInfo() {}
    //! Contains the genome, and information about the attractors in the network.
    AllBasins ab;
    //! The evolutionary generation at which this network evolved.
    unsigned int generation;
    //! The fitness of the network
    double fitness = 0.0;
    //! How much the fitness changed since the last genome
    double deltaF = 0.0;
    //! How many transitions (in ab) have changed since the last network.
    unsigned int numChangedTransitions = 0;
};

#endif // __BASINS_H__
