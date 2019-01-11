/*
 * Compute h(m) from the paper
 */

#include <iostream>
#include <sstream>

#include "lmp.h"
#include "xmath.h"
#include "primeswing.h"

using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;

#ifndef N_Genes
# define N_Genes 5
#endif

int main(int argc, char** argv)
{
    ulong nGenes = N_Genes;
    if (argc > 1) {
        nGenes = (ulong)atoi(argv[1]);
    }
    cerr << "nGenes: " << nGenes << endl;

    // Compute h(m) from the paper
    ulong N = nGenes * (1<<(nGenes-1));
    cerr << "N bits: " << N << endl;

    Xint N_minus_k_fac;
    Xint N_minus_m_fac;
    Xint N_minus_k_minus_m_fac;
    Xint N_fac;
    Xint num;
    long double num_dbl = 0.0;
    Xint denom;
    long double denom_dbl = 0.0;
    Xint h_m;
    long double h_m_dbl = 0.0;
    Xint zero;

    lmp::InitSetUi(N_minus_k_fac, 1);
    lmp::InitSetUi(N_minus_m_fac, 1);
    lmp::InitSetUi(N_minus_k_minus_m_fac, 1);
    lmp::InitSetUi(N_fac, 1);
    lmp::InitSetUi(num, 1);
    lmp::InitSetUi(denom, 1);
    lmp::InitSetUi(h_m, 1);
    lmp::InitSetUi(zero, 0);

    PrimeSwing::ParallelFactorial (N_fac, N);

    //cout << "# nGenes: " << nGenes << ", N=" << N << endl;
    cout << "k,m,h(m)" << endl;
    for (ulong k = 1; k<N; ++k) {
        PrimeSwing::ParallelFactorial (N_minus_k_fac, (N-k));
        // Compute up to 8 mutations away
        for (ulong m = 1; m <= N/2; ++m) {
            if (N < (k+m)) {
                continue;
            }
            PrimeSwing::ParallelFactorial (N_minus_m_fac, (N-m));
            lmp::Mul(num, N_minus_m_fac, N_minus_k_fac);
            PrimeSwing::ParallelFactorial (N_minus_k_minus_m_fac, (N-k-m));
            lmp::Mul(denom, N_fac, N_minus_k_minus_m_fac);
            if (lmp::Cmp (denom, zero) == 0) {
                cerr << "denominator is zero!" << endl;
                //cout << k << "," << m << ",NaN" << endl;
            } else {
                slong thecmp = lmp::Cmp (num, denom);
                if (thecmp > 0) {
                    lmp::Div(h_m, num, denom);
                    cout << k << "," << m << "," << h_m << endl;
                } else if (thecmp == 0) {
                    cout << k << "," << m << ",1" << endl;
                } else {
                    // thecmp < 0, so need a floating point output.
                    // This method fails for normal doubles
                    denom_dbl = 0.0;
                    stringstream ss;
                    ss << denom;
                    ss >> denom_dbl;
                    //cerr << "denom_dbl " << denom_dbl << " and num_dbl: " << num_dbl << endl;
                    stringstream ss2;
                    ss2 << num;
                    ss2 >> num_dbl;
                    h_m_dbl = num_dbl/denom_dbl;
                    cout << k << "," << m << "," << h_m_dbl << endl;
                }
            }
        }
    }

    lmp::Clear(N_minus_k_fac);
    lmp::Clear(N_minus_m_fac);
    lmp::Clear(N_minus_k_minus_m_fac);
    lmp::Clear(N_fac);
    lmp::Clear(num);
    lmp::Clear(denom);
    lmp::Clear(h_m);
    lmp::Clear(zero);

    return 0;
}
