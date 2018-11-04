/*
 * Show all of a set of combinations of size k drawn from a set size
 * n.
 */
#include <iostream>
#include <map>

#define N_Genes 5 // required for lib.h but unused
#include "lib.h"

#include "lmp.h"
#include <sstream>
#include <vector>

using namespace std;

int main (int argc, char** argv)
{
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " ngenes limitlen" << endl;
        return 1;
    }

    int ngenes = atoi(argv[1]);
    int n = (1<<ngenes);   // 2^ngenes
    int l = atoi(argv[2]); // Length of limit cycle

    int zmask = 0;
    if (argc > 3) {
        zmask = atoi(argv[3]);
    }

    int m = 0; // Col for preceding zero score count
    if (argc > 4) {
        m = atoi(argv[4]);
    }

    int comb[n]; // comb[i] is the index of the i-th element in the combination

    // Initialise comb to zero
    for (int i = 0; i < n; ++i) {
        comb[i] = 0;
    }

    // Setup comb for the initial combination
    for (int i = 0; i < l; ++i) {
        comb[i] = i;
    }

    // Print the first combination
    int combo_count = 0;
    cout << "c";
    cout.fill('0');
    cout.width(2);
    cout << combo_count++ <<  endl;

    map<float, unsigned int> scores;
    float score = 0.0f;
    int zs = 0;
    int prec_zcs = 0; // Contains a count of the sets which score zero in cols preceding col m
    int zcs = 0; // Count number of sets with a zero in col m, and not also a zero in col preceding col m.
    zs += printc_binary (comb, l, ngenes, zmask, score, m, prec_zcs, zcs);
    scores[score]++;
    printc (comb, l);

    // Generate and print all the other combinations
    while (next_combination(comb, l, n)) {
        cout << "c";
        cout.fill('0');
        cout.width(2);
        cout << combo_count++ <<  endl;
        zs += printc_binary (comb, l, ngenes, zmask, score, m, prec_zcs, zcs);
        scores[score] = scores[score] + 1;
        printc (comb, l);
        cout << "---\n";
    }

    if (zmask) {
        cout << "Number of zs is " << zs << endl;
        cout << "zmask was " << zmask << endl;
    }

    auto mi = scores.begin();
    while (mi != scores.end()) {
        cout << mi->first << "," << mi->second << endl;
        ++mi;
    }
    cout << "Total combinations: " << combo_count << endl;
    cout << "Numbers with zero score: " << scores[0.0f] << endl;
    cout << "Numbers with non-zero score: " << (combo_count - scores[0.0f]) << endl;
    cout << "Sets with zero score given in cols preceding m=" << m << ": " << prec_zcs << endl;
    cout << "Sets with zero score in m=" << m << " (and not zc preceding): " << zcs << endl;
    cout << "P(0) = " << (scores[0.0f]/(float)combo_count) << endl;
    cout << "P(!0) = " << (1.0f - scores[0.0f]/(float)combo_count) << endl;

#if 0
    // Last thing - compute probability by the theoretical method:
    Xint sets_choose_l;
    Xint m_choose_i;
    lmp::InitSetUi(sets_choose_l, 1);
    lmp::InitSetUi(m_choose_i, 1);

    // The x, which I have yet to determine the pattern for.
    int x = ngenes - l; // wrong

    lmp::BinomialUiUi(half_bitw_ch_l, n>>1, l);
    cout << (n>>1) << " choose " << l << ": " << half_bitw_ch_l << endl;
    stringstream ss1;
    ss1 << half_bitw_ch_l;
    double half_bitw_ch_l_dbl = 0.0;
    ss1 >> half_bitw_ch_l_dbl;
    lmp::BinomialUiUi(bitw_ch_l, n, l);
    cout << n << " choose " << l << ": " << bitw_ch_l << endl;
    stringstream ss2;
    ss2 << bitw_ch_l;
    double bitw_ch_l_dbl = 0.0;
    ss2 >> bitw_ch_l_dbl;

    Xint two_to_m_ch_l;
    Xint two_to_p_ch_l;
    lmp::InitSetUi(two_to_m_ch_l, 0);
    lmp::InitSetUi(two_to_p_ch_l, 0);

    // Compute dependent probabilities
    vector<double> pzeros;
    double mustbes = 0.0;
    double num_offset = 0.0;
    double P0 = 0.0;
    double PNOT0 = 1.0;

    int two_to_m_ch_l_int = 0;
    int two_to_p_ch_l_int = 0;

    for (unsigned int col = 0; col < ngenes; ++col) {
        cout << "---" << endl;
        unsigned int m = col; // n cols left
        unsigned int p = ngenes - col - 1; // n cols right

        lmp::BinomialUiUi (two_to_m_ch_l, (1<<m), l);
        cout << "(2^m choose l): " << (1<<m) << " choose " << l <<  " = " << two_to_m_ch_l << endl;
        stringstream ssns;
        ssns << two_to_m_ch_l;
        ssns >> two_to_m_ch_l_int;

        lmp::BinomialUiUi (two_to_p_ch_l, (1<<p), l);
        cout << "(2^p choose l): " << (1<<p) << " choose " << l <<  " = " << two_to_p_ch_l << endl;
        stringstream ssns2;
        ssns2 << two_to_p_ch_l;
        ssns2 >> two_to_p_ch_l_int;

        num_offset += static_cast<double>(two_to_p_ch_l_int * two_to_m_ch_l_int);
        cout << "Numerator offset: -" << num_offset << endl;

        double prob_num = half_bitw_ch_l_dbl - num_offset;
        double prob_denom = bitw_ch_l_dbl + mustbes;

        cout << "numerator: " << prob_num << endl;
        cout << "denomenator: " << prob_denom << endl;

        pzeros.push_back (prob_num/prob_denom);
        cout << "P(ZC"<<(col+1)<<"|!ZC[1->"<<col<<"]) = " << pzeros.back() << endl;

        mustbes = mustbes - prob_num;

        PNOT0 = PNOT0 * (1.0-pzeros.back());
    }
    cout << "Calculated P(!0) = " << PNOT0 << endl;
#endif

    return 0;
}
