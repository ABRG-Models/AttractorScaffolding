/*!
 * Functions to determine the number of fit mutated genomes that exist
 * a Hamming distance m (or h) away from a given, fit genome.
 */

#ifndef _MUTATION_H_
#define _MUTATION_H_

#include <math.h>

#ifndef __FITNESS_FUNCTION__
#error "#include a fitness.h before #including mutations.h to ensure evaluate_fitness() is available"
#endif

/*!
 * Return the number of fit mutations of genome at a Hamming
 * distance of h. Exponentially costly in h, I think.
 */
pair<unsigned int, double>
num_fit_mutations (const array<genosect_t, N_Genes>& genome, unsigned int h)
{
    // Return variables
    unsigned int numfit = 0;
    double fitness_sum = 0.0f;

    // The length of the genome
    unsigned int l_genome = N_Genes * (1<<N_Ins);

    // A genome that gets flipped from the original genome and then evaluated.
    array<genosect_t, N_Genes> flipped_genome;

    // This makes combo long enough for h all the way up to the full
    // width of the genome.  combo[i] is the index of the i-th element
    // in the combination
    int combo[N_Genes * (1<<N_Ins)];

    // Setup combo for the initial combination - first h bits set to
    // 1. Set everything else to 0
    for (int i = 0; i < static_cast<int>(h); ++i) { combo[static_cast<unsigned int>(i)] = i; }
    for (unsigned int i = h; i < l_genome; ++i) { combo[i] = 0; }

    // Generate and evaluate all the combinations
    bool finished = false;
    while (!finished) {
        // printc (combo, h);

        // Initialise flipped_genome
        copy_genome (genome, flipped_genome);

        // Work out flipped_genome[i]s and then compute fitness
        for (unsigned int i = 0; i < N_Genes; ++i) {
            for (unsigned int j = 0; j < h; ++j) {
                if (combo[j] >= static_cast<int>(i*(1<<N_Ins))
                    && combo[j] < static_cast<int>((i+1)*(1<<N_Ins))) {
                    // Then combo[j] is an index that is in genosect[i] so flip this bit.
                    unsigned int bit_to_flip = combo[j] - (i*(1<<N_Ins));
                    flipped_genome[i] ^= (0x1 << bit_to_flip);
                }
            }
        }

        float f = evaluate_fitness (flipped_genome);
        if (f > 0.0) {
            ++numfit;
            fitness_sum += (double)f;
        }

        if (!next_combination (combo, h, l_genome)) {
            finished = true;
        }
    }

    DBG ("Numfit:" << numfit << " sum of fitness: " << fitness_sum);
    return make_pair(numfit, fitness_sum);
}

/*!
 * Compute an estimate of the number of fit mutations at a given
 * Hamming distance, hd, by making a number of samples, num_samples.
 */
pair<unsigned int, double>
num_fit_mutations_sample (const array<genosect_t, N_Genes>& genome,
                          unsigned int hd, unsigned int num_samples)
{
    unsigned int numfit = 0;
    double fitness_sum = 0.0f;

    unsigned int n_in_gs = (1 << N_Ins);
    vector<unsigned int> flipbit (hd, 0);
    array<genosect_t, N_Genes> flip_mask;
    array<genosect_t, N_Genes> flipped_genome;

    set<array<genosect_t, N_Genes> > masks;

    for (unsigned int s = 0; s < num_samples; ++s) {

        copy_genome (genome, flipped_genome);
        zero_genome (flip_mask);

        // Get random numbers to determine flipping
        //for (unsigned int h = 0; h < hd; ++h) {
        unsigned int h = 0;
        unsigned int num_miss = 0;
        while (h<hd) {
            // Should be in range 0 to 80 for N_Genes==5
            float f = randFloat() * N_Genes * n_in_gs;
            flipbit[h] = (unsigned int)floorf (f);
            DBG2 ("flipbit[" << h << "]: " << flipbit[h]);
            unsigned int fb = (unsigned int)floorf (f);
            unsigned int fb_genosect = fb / n_in_gs;
            unsigned int fb_offset = fb % n_in_gs;
            // mask_part is the new part of the mask to flip
            genosect_t mask_part = 0x1 << fb_offset;
            // Make sure we didn't already flip this one!
            if ((flip_mask[fb_genosect] & mask_part) != mask_part) {
                // Set bit fb_offset in flip_mask[fb_genosect];
                flip_mask[fb_genosect] |= mask_part;
                ++h;
                DBG2 ("Added to flip_mask...");
            } else {
                // Do nothing, make another sample from the RNG to flip another bit.
                DBG2("It's a miss.");
                ++num_miss;
            }
        }
#ifdef DEBUG2
        if (num_miss > 0) {
            DBG ("Number of misses creating flip_mask was: " << num_miss);
        }
#endif
        // Check we didn't already choose this one.
        DBG2 ("Check if flip_mask already seen...");
        if (masks.count(flip_mask) == 0) {
            masks.insert(flip_mask);
            // Now do the flipping, genome section by genome section
#pragma omp simd
            for (unsigned int i=0; i<N_Genes; ++i) {
                flipped_genome[i] ^= flip_mask[i];
            }
            // Now evaluate the fitness
            float f = evaluate_fitness (flipped_genome);
            if (f > 0.0) {
                ++numfit;
                fitness_sum += (double)f;
            }
        } else {
            // We found a duplicate mask, so decrement s to make sure we do one more random sample.
            DBG2 ("Flip mask seen :(");
            --s;
        }
    }
    DBG ("Numfit:" << numfit << " sum of fitness: " << fitness_sum);
    return make_pair (numfit, fitness_sum);
}

/*!
 * Create a random genome and evolve it into a maximally fit genome.
 */
array<genosect_t, N_Genes>
evolve_new_genome (void)
{
    // Holds the genome and a copy of it.
    array<genosect_t, N_Genes> refg;
    array<genosect_t, N_Genes> newg;

    random_genome (refg);
    float a = evaluate_fitness (refg);

    unsigned int gen = 0;
    // Test fitness to determine whether we should evolve.
    while (a < 1.0f) {
        copy_genome (refg, newg);
        evolve_genome (newg);
        ++gen;
        float b = evaluate_fitness (newg);
        if (a > b) {
            // New fitness <= old fitness
        } else {
            // Copy new fitness to ref
            a = b;
            // Copy new to reference
            copy_genome (newg, refg);
        }
    }
    DBG ("It took " << gen << " generations to evolve this genome");

    return refg;
}

#endif // _MUTATION_H_
