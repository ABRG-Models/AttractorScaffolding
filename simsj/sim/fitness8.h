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

#define FF_NAME "ff8"

// Evaluate the expression level of the limit cycle and return in the
// array.
array<double, N_Genes>
evaluate_expression (array<genosect_t, N_Genes>& genome, state_t state)
{
    array<double, N_Genes> expression;
    for (unsigned int j = 0; j < N_Genes; ++j) { expression[j] = 0.0; }

#ifdef DEBUGF
    DBGF ("Evaluating expression for initial state " << state_str(state));
#endif

    state_t state_last = state_t_unset;
    set<state_t> visited;
    visited.insert (state); // insert starting state
    for (;;) {
        state_last = state;
        compute_next (genome, state);

        if (visited.count (state)) {
            // Already visited this state so it's a limit cycle

            if (state == state_last) {
                // Point attractor. Put state into array<double,
                // N_Genes>
                for (unsigned int j = 0; j < N_Genes; ++j) {
                    expression[j] = static_cast<double>((state >> j) & 0x1);
                }
            } else {
                // "Circular" limit cycle

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

                // Now have the set lc; can work out its expression.
                set<state_t>::const_iterator i = lc.begin();
                while (i != lc.end()) {
                    for (unsigned int j = 0; j < N_Genes; ++j) {
                        expression[j] += static_cast<double>(((*i) >> j) & 0x1);
                    }
                    ++i;
                }
                // Divide down now.
                for (unsigned int j = 0; j < N_Genes; ++j) {
                    expression[j] /= static_cast<double>(lc_len);
                }
            }
            break;
        }
        visited.insert (state);
    }

    return expression;
}

//#define MULTIPLIC 1
/*!
 * For the passed-in genome, find its final state, starting from the
 * anterior state initial_ant and the posterior state initial_pos
 * (stored in global variables). Return a fitness specifier for the
 * genome.
 *
 * This function examines the limit cycles that are arrived at from
 * the two initial states. The fitness is then computed according to
 * whether the final anterior and posterior states deliver a gradient
 * in the correct directions.
 */
double
evaluate_fitness (array<genosect_t, N_Genes>& genome)
{
    array<double, N_Genes> exp_ant = evaluate_expression (genome, initial_ant);
    array<double, N_Genes> exp_pos = evaluate_expression (genome, initial_pos);

    // Need to determine, for each gene, whether we have:
    //
    // case 0: target_ant==1 && target_pos==1
    //
    // case 1: target_ant==0 && target_pos==0
    //
    // case 2: target_ant==1 && target_pos==0
    //
    // case 3: target_ant==0 && target_pos==1
    //
    // As this allows us to compute a per-gene pair score.

    // Assume target_ant and target_pos have all been state-masked already.
    state_t a_and_p =                           (target_ant & target_pos);     // case 0
    state_t not_a_or_p =                       ~(target_ant | target_pos) & state_mask; // case 1
    state_t a_and_a_xor_p =      target_ant & (target_ant ^ target_pos); // case 2
    state_t p_and_a_xor_p =      target_pos & (target_ant ^ target_pos); // case 3?

#ifdef DEBUGF
    DBGF ("Target ant:         " << state_str (target_ant));
    DBGF ("Target pos:         " << state_str (target_pos));
    DBGF ("Case 0 (a=1 & p=1): " << state_str (a_and_p));
    DBGF ("Case 1 (a=0 & p=0): " << state_str (not_a_or_p));
    DBGF ("Case 2 (a=1 & p=0): " << state_str (a_and_a_xor_p));
    DBGF ("Case 3 (a=0 & p=1): " << state_str (p_and_a_xor_p));
#endif

    array<double, N_Genes> fitnesses;
    for (unsigned int j=0; j<N_Genes; ++j) {

        if ((a_and_p >> j) & 0x1) { // case 0
            fitnesses[j] = 0.5 * (exp_ant[j] + exp_pos[j]);

        } else if ((not_a_or_p >> j) & 0x1) { // case 1
            fitnesses[j] = 0.5 * (2 - exp_ant[j] - exp_pos[j]);

        } else if ((a_and_a_xor_p >> j) & 0x1) { // case 2
            DBGF ("Gene " << j << " case 2");
#ifndef MULTIPLIC
            fitnesses[j] = 0.5 * (exp_ant[j] - exp_pos[j] + 1);
#else
            fitnesses[j] = exp_ant[j] - exp_pos[j];
            if (fitnesses[j] > 0.0) {
                // Nothing further to do
            } else {
                // If the gradient is in the wrong direction, set fitness to 0.
                fitnesses[j] = 0.0;
            }
#endif
        } else if ((p_and_a_xor_p >> j) & 0x1) { // case 3
            DBGF ("Gene " << j << " case 3");
#ifndef MULTIPLIC
            // This gives a fitness in range 0 to 1, with 0 to 0.5
            // given for gradients in the wrong direction.
            fitnesses[j] = 0.5 * (exp_pos[j] - exp_ant[j]);
#else
            fitnesses[j] = exp_pos[j] - exp_ant[j];
            if (fitnesses[j] > 0.0) {
                // Nothing further to do
            } else {
                // If the gradient is in the wrong direction, set fitness to 0.
                fitnesses[j] = 0.0;
            }
#endif
        } else {
            cerr << "Error in " << FF_NAME << " evaluate_fitness()" << endl;
            fitnesses[j] = 0.0;
        }
    }

#ifdef MULTIPLIC
    double fitness = 1.0;
    for (unsigned int j=0; j<N_Genes; ++j) {
        DBGF (" Gene " << j << " has expression ant/pos: " << exp_ant[j] << "/" << exp_pos[j] << " contributes: " << fitnesses[j]);
        fitness *= fitnesses[j];
    }
    fitness /= N_Genes;
#else
    double fitness = 0.0;
    for (unsigned int j=0; j<N_Genes; ++j) {
        DBGF (" Gene " << j << " has expression ant/pos: " << exp_ant[j] << "/" << exp_pos[j] << " contributes: " << fitnesses[j]);
        fitness += fitnesses[j];
    }
    fitness /= N_Genes;
#endif

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

#endif // __FITNESS_FUNCTION__
