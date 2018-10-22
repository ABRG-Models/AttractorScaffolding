/*!
 * This is a library of functions for the Boolean networks model of
 * Wilson et al. 2018.
 *
 * Author: Seb James
 */

#ifndef __LIB_H__
#define __LIB_H__

#include <bitset>
#include <array>
#include <iostream>
#include <stdlib.h>

using namespace std;

/*!
 * Debugging/informational macros.
 */
#ifdef DEBUG
# define DBG(s)  cout << "DBG: " << __FUNCTION__ << ": " << s << endl;
#else
# define DBG(s)
#endif
#ifdef DEBUG2
# define DBG2(s)  cout << "DBG2: " << __FUNCTION__ << ": " << s << endl;
#else
# define DBG2(s)
#endif
#define LOG(s)  cout << "LOG: " << s << endl;

#ifndef N_Genes
# error 'Number of genes parameter N_Genes must be defined'
#endif
#define N_Ins       (N_Genes-1)

/*!
 * The genome has a section for each 'molecule'. The length of the
 * section of each molecule is 2^N_Ins. 32 bit width sections are
 * enough for N_Genes <= 6. 64 bit width sections ok for N_Genes <= 7.
 */
typedef unsigned int genosect_t;

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
 * Probability of flipping during evolution.
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
 * Set the global target states for anterior and posterior positions.
 */
//@{
#if N_Genes < 4
#error "You'll need to set up target_ant/target_pos suitably for N_Genes < 4"
#endif
#if N_Genes == 4
state_t target_ant = 0xa; // 1010 or 10 dec
state_t target_pos = 0x5; // 0101 or 5 dec
#endif
#if N_Genes == 5
state_t target_ant = 0x15; // 10101 or 21 dec
state_t target_pos = 0xa;  // 01010 or 10 dec
#endif
#if N_Genes == 6
state_t target_ant = 0x2a; // 101010
state_t target_pos = 0x15; // 010101
#endif
#if N_Genes > 6
#error "You'll need to set up target_ant/target_pos suitably for N_Genes > 6"
#endif
//@}

// Initial anterior and posterior states:
#if N_Genes == 4
state_t initial_ant = 0x8;  // 1000b;
state_t initial_pos = 0x0;  // 0000b;
#endif
#if N_Genes == 5
state_t initial_ant = 0x10; // 10000b;
state_t initial_pos = 0x0;  // 00000b;
#endif
#if N_Genes == 6
state_t initial_ant = 0x20; // 100000b;
state_t initial_pos = 0x0;  // 000000b;
#endif

/*!
 * Initialise the masks based on the value of N_Genes
 */
void
masks_init (void)
{
#ifdef DEBUG2
    // 2 ^ (N_Genes - 1) == 1 << (N_Genes-1) == 1 << N_Ins
    unsigned int l_genome = N_Genes * (1 << N_Ins);
    DBG2 ("N_Genes: " << N_Genes << " N_Ins: " << N_Ins << " length of genome: " << l_genome);
#endif

    // Set up globals. Set N_Genes-1 bits to the high position for the lo_mask
    lo_mask_start = 0x0;
    for (unsigned int i = 0; i < N_Genes-1; ++i) {
        lo_mask_start |= 0x1 << i;
    }
    hi_mask_start = 0xff & (0xff << N_Genes);

    genosect_mask = 0x0;
    for (unsigned int i = 0; i < (1<<N_Ins); ++i) { // 1<<N is the same as 2^N
        genosect_mask |= (0x1 << i);
    }
    DBG2 ("genosect_mask: 0x" << hex << genosect_mask << dec); // 65535 is 16 bits
}

#if N_Genes == 5
/*!
 * This is the genome given in Fig 1 of the paper for N_Genes=5:
 */
array<genosect_t, N_Genes>
selected_genome (void)
{
    // The one we'll use in the paper (currently)
    array<genosect_t, N_Genes> genome = {{ 0x8a3b, 0x467e, 0xdba0, 0x6253, 0xe0f }};
    return genome;
}

array<genosect_t, N_Genes>
selected_genome1 (void)
{
    // Has limit cycle attractor. Was previously going to be the selected genome
    array<genosect_t, N_Genes> genome = {{ 0xa036, 0x451f, 0x7f08, 0x203c, 0xdf52 }};
    return genome;
}
#endif

#ifdef SET_KNOWN_GENOME_BITS
/*!
 * Given a state for N_Genes (that is, a number with N_Genes bits),
 * compute the inputs for each gene.
 */
array<state_t, N_Genes>
compute_next_inputs (state_t state)
{
    array<state_t, N_Genes> inputs;
    state_t lo_mask = lo_mask_start;
    state_t hi_mask = hi_mask_start;
    for (unsigned int i = 0; i < N_Genes; ++i) {
        inputs[i] = (state & lo_mask) | ((state & hi_mask) >> 1);
        hi_mask = (hi_mask >> 1) | state_t_top_bit;
        lo_mask >>= 1;
    }
    return inputs;
}
#endif

/*!
 * Given a state for N_Genes, and a genome, compute the next
 * state. This is "develop" rather than "evolve".
 */
void
compute_next (array<genosect_t, N_Genes>& genome, state_t& state)
{
    array<state_t, N_Genes> inputs;
    state_t lo_mask = lo_mask_start;
    state_t hi_mask = hi_mask_start;

#ifdef DEBUG
    bitset<8> bs_st(state);
    DBG2 ("state: " << bs_st);
#endif
    for (unsigned int i = 0; i < N_Genes; ++i) {
        inputs[i] = (state & lo_mask) | ((state & hi_mask) >> 1);
        hi_mask = (hi_mask >> 1) | 0x80;
        lo_mask >>= 1;
#ifdef DEBUG
        bitset<8> bs_in(inputs[i]);
        DBG2 ("input[" << i << "]: " << bs_in << " or 0x" << hex << (unsigned int)inputs[i]<< dec);
#endif
    }

    // Now reset state and compute new values:
    state = 0x0;

    // State a anterior is genome[inps[0]] etc
    for (unsigned int i = 0; i < N_Genes; ++i) {
        DBG2 ("Setting state for gene " << i);
        genosect_t gs = genome[i];
        genosect_t inpit = (0x1 << inputs[i]);
#ifdef DEBUG
        bitset<16> bs_gs(gs);
        bitset<16> bs_inpit(inpit);
        DBG2 ("genome sect: "<< bs_gs);
        DBG2 ("iterator into that sect: " << bs_inpit << "...");
#endif
        state_t num = ((gs & inpit) ? 0x1 : 0x0);
        DBG2 ("... which gives " << (unsigned int)num);
        if (num) {
            state |= (0x1 << (N_Ins-i));
        } else {
            state &= ~(0x1 << (N_Ins-i));
        }
    }
}

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

void
show_state (const state_t& state)
{
    for (unsigned int i = 0; i < N_Genes; ++i) {
        cout << (char)((char)'a'+(char)i) << " ";
    }
    cout << endl;
    cout << state_str (state) << endl;
}

/*
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

/*
 * Convert from my array of genosect_t form for genome to the long
 * double form used by Stuart's code. Untested; no idea if it works.
 */
long double
genome2int (array<genosect_t, N_Genes>& genome)
{
    __uint128_t u = 0;
    unsigned int shift = 0;
    for (unsigned int i = 0; i < N_Genes; ++i) {
        __uint128_t g = genome[i];
        u |= (g << shift);
        shift+=16;
    }
    return (long double)u;
}

/*!
 * Produce the string of 1 and 0 chars to match the format Dan
 * provides genomes in.
 */
string
genome2str (array<genosect_t, N_Genes>& genome)
{
    stringstream rtn;
    for (unsigned int i = 0; i < N_Genes; ++i) {
        for (unsigned int j = 0; j < (1 << N_Ins); ++j) {
            genosect_t mask = 0x1 << j;
            rtn << ((genome[i]&mask) >> j);
        }
    }
    return rtn.str();
}

array<genosect_t, N_Genes>
str2genome (const string& s)
{
    array<genosect_t, N_Genes> g;
    // Init g
    for (unsigned int i = 0; i < N_Genes; ++i) {
        g[i] = 0x0;
    }

    // Check length of string.
    unsigned int l = s.length();
    unsigned int l_genosect = 1 << N_Ins;
    unsigned int l_genome = N_Genes * l_genosect;
    if (l == l_genome) {
        LOG ("String has " << l_genome << " bit chars as required...");
    } else {
        return g;
    }

    for (unsigned int i = 0; i < N_Genes; ++i) {
        for (unsigned int j = 0; j < l_genosect; ++j) {
            bool high = (s[j + i*l_genosect] == '1');
            if (high) {
                g[i] |= 0x1 << j;
            } // else do nothing.
        }
    }

    return g;
}

/*
 * Output genome in hex.
 */
string
genome_id (const array<genosect_t, N_Genes>& genome)
{
    stringstream ss;
    ss << hex;
    bool first = true;
    for (unsigned int i = 0; i<N_Genes; ++i) {
        if (first) {
            first = false;
            ss << genome[i];
        } else {
            ss << "-" << genome[i];
        }
    }
    ss << dec;
    return ss.str();
}

/*!
 * A debugging aid to display the genome in a little table.
 */
void
show_genome (const array<genosect_t, N_Genes>& genome)
{
    cout << "Genome:" << endl;
    bool first = true;
    for (unsigned int i = 0; i<N_Genes; ++i) {
        if (first) {
            first = false;
            cout << (char)('a'+i);
        } else {
            cout << "     " << (char)('a'+i);
        }
    }
    cout << endl;
    cout << hex;
    first = true;
    for (unsigned int i = 0; i<N_Genes; ++i) {
        if (first) {
            first = false;
            cout << "0x" << genome[i];
        } else {
            cout << " 0x" << genome[i];
        }
    }
    cout << dec << endl;
    cout << "Genome table:" << endl;
    cout << "input  output" << endl;
    for (unsigned int i = N_Ins; i > 0; --i) {
        cout << (i-1);
    }
    cout << "   ";
    for (unsigned int i = 0; i < N_Genes; ++i) {
        cout << i << " ";
    }
    cout << "<-- for input, bit posn; for output, array index";
    cout << endl << "----------------" << endl;
#if N_Genes == 5
    cout << "1234   a b c d e <-- 1,2,3,4 is i ii iii iv in Fig 1." << endl;
#else
    for (unsigned int i = 0; i<N_Ins; ++i) {
        cout << i;
    }
    cout << "  ";
    for (unsigned int i = 0; i<N_Genes; ++i) {
        cout << " " << (char)('a'+i);
    }
    cout << endl;
#endif
    cout << "----------------" << endl;

    for (unsigned int j = 0; j < (1 << N_Ins); ++j) {
        cout << bitset<N_Ins>(j) << "   ";
        for (unsigned int i = 0; i < N_Genes; ++i) {
            genosect_t mask = 0x1 << j;
            cout << ((genome[i]&mask) >> j) << " ";
        }
        cout << endl;
    }
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
 * Set the genome g to zero.
 */
void
zero_genome (array<genosect_t, N_Genes>& g)
{
#pragma omp simd
    for (unsigned int i = 0; i < N_Genes; ++i) {
        g[i] = 0;
    }
}

/*!
 * Copy the contents of @from to @to
 */
void
copy_genome (const array<genosect_t, N_Genes>& from, array<genosect_t, N_Genes>& to)
{
#pragma omp simd
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
#ifdef DEBUG
    unsigned int numflipped = 0;
#endif
    for (unsigned int i = 0; i < N_Genes; ++i) {
        genosect_t gsect = genome[i];
        for (unsigned int j = 0; j < (1<<N_Ins); ++j) {
            if (randFloat() < pOn) {
                // Flip bit j
#ifdef DEBUG
                ++numflipped;
#endif
                gsect ^= (0x1 << j);
            }
        }
        genome[i] = gsect;
    }
    DBG ("Num flipped: " << numflipped);
}

void
random_genome (array<genosect_t, N_Genes>& genome)
{
    /*
     * Initialise a fully random genome
     */
    for (unsigned int i = 0; i < N_Genes; ++i) {
        genome[i] = ((genosect_t) rand()) & genosect_mask;
    }
}

/*!
 * Generate a random genome.
 */
array<genosect_t, N_Genes>
random_genome (void)
{
    /*
     * Initialise a fully random genome
     */
    array<genosect_t, N_Genes> genome;
    for (unsigned int i = 0; i < N_Genes; ++i) {
        genome[i] = ((genosect_t) rand()) & genosect_mask;
    }

#ifdef SET_KNOWN_GENOME_BITS
    /*
     * Now set the bits in the genome that we can deduce.
     */
    array<state_t, N_Genes> inp_target_ant = compute_next_inputs (target_ant);
    array<state_t, N_Genes> inp_target_pos = compute_next_inputs (target_pos);

    for (unsigned int i = 0; i < N_Genes; ++i) {

        genosect_t a = ((target_ant >> i) & 0x1);
        if (a) {
            genosect_t b = a << inp_target_ant[i];
#ifdef DEBUG
            bitset<16> bs (b);
            DBG2 ("(ant) For genome["<<i<<"] we OR  with: " << bs);
#endif
            genome[i] = genome[i] | b;
        } else {
            genosect_t b = ~(0x1 << inp_target_ant[i]);
#ifdef DEBUG
            bitset<16> bs (b);
            DBG2 ("(ant) For genome["<<i<<"] we AND with: " << bs);
#endif
            genome[i] = genome[i] & b;
        }

        a = ((target_pos >> i) & 0x1);
        if (a) {
            genosect_t b = a << inp_target_pos[i];
#ifdef DEBUG
            bitset<16> bs (b);
            DBG2 ("(pos) For genome["<<i<<"] we OR  with: " << bs);
#endif
            genome[i] = genome[i] | b;
        } else {
            genosect_t b = ~(0x1 << inp_target_pos[i]);
#ifdef DEBUG
            bitset<16> bs (b);
            DBG2 ("(pos) For genome["<<i<<"] we AND with: " << bs);
#endif
            genome[i] = genome[i] & b;
        }

        bitset<16> bs_gn(genome[i]);
        DBG2 ("genome[" << i << "] = " << bs_gn);
    }
#endif // SET_KNOWN_GENOME_BITS

    return genome;
}

// Using intrinsics for computing Hamming distances
#include <immintrin.h>

/*!
 * Computes the Hamming distance between state and target. This is
 * equal to the number of bit positions at which state and target
 * differ.
 */
state_t
compute_hamming (state_t state, state_t target)
{
#ifdef NO_INSTRINSICS
#ifdef DEBUG2
    bitset<N_Genes> bs_st(state);
    bitset<N_Genes> bs_tar(target);
    DBG2 ("State: " << bs_st << " Target:"  << bs_tar);
#endif
    state_t hamming = N_Genes;
    if (state != target) {
        for (unsigned int i = 0; i < N_Genes; ++i) {
            // Bit i in state
            DBG2 ("st: " << ((state & (0x1 << i))>>i) << " targ: " << ((target & (0x1 << i))>>i));
            if (((state & (0x1 << i))>>i) == ((target & (0x1 << i))>>i)) {
                hamming--;
            }
        }
    } else {
        hamming = 0;
    }
    DBG2 ("hamming: " << (unsigned int)hamming);
    return hamming;
#else
    // For this very short type, a lookup is probably faster than this intrinsic:
    state_t bits = state ^ target;
    unsigned int hamming = _mm_popcnt_u32 ((unsigned int)bits);
    return static_cast<state_t>(hamming);
#endif
}

/*!
 * Compute Hamming distance between two genomes.
 */
unsigned int
compute_hamming (const array<genosect_t, N_Genes>& g1,
                 const array<genosect_t, N_Genes>& g2)
{
    unsigned int hamming = 0;
    for (unsigned int i = 0; i < N_Genes; ++i) {
        genosect_t bits = g1[i] ^ g2[i]; // XOR
        hamming += _mm_popcnt_u32 ((unsigned int)bits);
    }
    return hamming;
}

/*!
 * This function obtained from
 * https://compprog.wordpress.com/2007/10/17/generating-combinations-1/
 *
 * next_combination (int comb[], int k, int n)
 *  Generates the next combination of n elements as k after comb
 *
 *  comb => the previous combination ( use (0, 1, 2, ..., k) for first)
 *  k => the size of the subsets to generate
 *  n => the size of the original set
 *
 *  Returns: 1 if a valid combination was found
 *           0, otherwise
 */
int
next_combination (int comb[], int k, int n)
{
    int i = k - 1;
    ++comb[i];
    while ((i >= 0) && (comb[i] >= n - k + 1 + i)) {
        --i;
        ++comb[i];
    }

    if (comb[0] > n - k) /* Combination (n-k, n-k+1, ..., n) reached */
        return 0; /* No more combinations can be generated */

    /* comb now looks like (..., x, n, n, n, ..., n).
    Turn it into (..., x, x + 1, x + 2, ...) */
    for (i = i + 1; i < k; ++i)
        comb[i] = comb[i - 1] + 1;

    return 1;
}

/*!
 * Prints out a combination like {1, 2} for debugging
 */
void
printc (int comb[], int k)
{
    printf("{");
    int i;
    for (i = 0; i < k; ++i)
        printf("%d, ", comb[i] + 1);
    printf("\b\b}\n");
}

#endif // __LIB_H__
