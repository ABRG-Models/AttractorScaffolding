/*
 * Show all of a set of combinations of size k drawn from a set size
 * n.
 */
#include <iostream>
#include <fstream>
#include <sstream>

#define N_Genes 5 // required for lib.h but unused
#include "lib.h"

#include "lmp.h"

long double
xint_to_double (Xint& xi)
{
    long double d;
    std::stringstream ss;
    ss << xi;
    ss >> d;
    return d;
}

using namespace std;

int main (int argc, char** argv)
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " ngenes" << endl;
        return 1;
    }

    int ngenes = atoi(argv[1]);
    //int n = (1<<ngenes);   // 2^ngenes

    ofstream fout;
    stringstream fpath;
    fpath << "./data/pnot0_n" << ngenes << ".csv";
    fout.open (fpath.str().c_str(), ios::out|ios::trunc);

    if (!fout.is_open()) {
        cerr << "Failed to open file " << fpath.str() << endl;
        return 1;
    }

    fout << "l,p(!0),p(0)" << endl;

    long double p0 = 0.0;
    long double pnot0 = 1.0;

    for (int l = 1; l<=(1<<(ngenes)); ++l) {

        Xint two_to_n_choose_l;
        Xint two_to_n_over_2_choose_l;
        lmp::InitSetUi (two_to_n_choose_l, 1);
        lmp::InitSetUi (two_to_n_over_2_choose_l, 1);

        long double two_to_n_choose_l_d = 0.0;
        long double two_to_n_over_2_choose_l_d = 0.0;
        long double two_to_n_minus_1_over_2i_choose_l_d = 0.0;
        long double two_to_n_over_2i_choose_l_d = 0.0;
        long double m_choose_i_d = 0.0;
        long double numsum_d = 0.0;
        long double denomsum_d = 0.0;

        // Once only:
        lmp::BinomialUiUi(two_to_n_over_2_choose_l, 1<<(ngenes-1), l);
        two_to_n_over_2_choose_l_d = xint_to_double (two_to_n_over_2_choose_l);

        lmp::BinomialUiUi(two_to_n_choose_l, 1<<ngenes, l);
        two_to_n_choose_l_d = xint_to_double (two_to_n_choose_l);

        long double col0 = two_to_n_over_2_choose_l_d / two_to_n_choose_l_d;
        pnot0 = 1.0-col0;
        //cout << "P(ZC0) = " << col0 << endl;
        //cout << "P(!ZC0) = " << pnot0 << endl;

        for (ulong col = 1; col < (ulong)ngenes; ++col) {
            numsum_d = 0.0;
            denomsum_d = 0.0;
            // Compute numerator sum
            for (ulong i = 1; i <= col; ++i) {

                Xint m_choose_i;
                Xint two_to_n_minus_1_over_2i_choose_l;
                lmp::InitSetUi (m_choose_i, 1);
                lmp::InitSetUi (two_to_n_minus_1_over_2i_choose_l, 1);

                lmp::BinomialUiUi(m_choose_i, col, i);
                m_choose_i_d = xint_to_double (m_choose_i);
                //cout << col << " choose " << i << " = " << m_choose_i_d << endl; // crash?!

                lmp::BinomialUiUi(two_to_n_minus_1_over_2i_choose_l, static_cast<ulong>(1<<(ngenes-1-i)), static_cast<ulong>(l));

                //cout << static_cast<ulong>(1<<(ngenes-1-i)) << " choose " << static_cast<ulong>(l) << " = "
                //     << two_to_n_minus_1_over_2i_choose_l << endl;
                two_to_n_minus_1_over_2i_choose_l_d = xint_to_double (two_to_n_minus_1_over_2i_choose_l);
                // +ve for 1, -ve for 2 etc
                bool negative = (i%2==0);
                if (negative) {
                    //cout << "numsum_d -= " << m_choose_i_d << " * " << two_to_n_minus_1_over_2i_choose_l_d << endl;
                    numsum_d -= m_choose_i_d * two_to_n_minus_1_over_2i_choose_l_d;
                } else {
                    //cout << "numsum_d += " << m_choose_i_d << " * " << two_to_n_minus_1_over_2i_choose_l_d << endl;
                    numsum_d += m_choose_i_d * two_to_n_minus_1_over_2i_choose_l_d;
                }
                lmp::Clear(m_choose_i);
                lmp::Clear(two_to_n_minus_1_over_2i_choose_l);
            }

            // Compute denomenator sum
            for (ulong i = 1; i <= col; ++i) {

                Xint m_choose_i;
                lmp::InitSetUi (m_choose_i, 1);
                Xint two_to_n_over_2i_choose_l;
                lmp::InitSetUi (two_to_n_over_2i_choose_l, 1);

                lmp::BinomialUiUi(m_choose_i, col, i);
                m_choose_i_d = xint_to_double (m_choose_i);
                lmp::BinomialUiUi(two_to_n_over_2i_choose_l, static_cast<ulong>(1<<(ngenes-i)), static_cast<ulong>(l));
                two_to_n_over_2i_choose_l_d = xint_to_double (two_to_n_over_2i_choose_l);
                // +ve for 1, -ve for 2 etc
                bool negative = (i%2==0);
                if (negative) {
                    denomsum_d -= m_choose_i_d * two_to_n_over_2i_choose_l_d;
                    //cout << "denomsum_d -= " << m_choose_i_d << " * " << two_to_n_over_2i_choose_l_d << endl;
                } else {
                    denomsum_d += m_choose_i_d * two_to_n_over_2i_choose_l_d;
                    //cout << "denomsum_d += " << m_choose_i_d << " * " << two_to_n_over_2i_choose_l_d << endl;
                }
                lmp::Clear(m_choose_i);
                lmp::Clear(two_to_n_over_2i_choose_l);
            }

            // Compute probability
            //cout << "Probability sum: " << two_to_n_over_2_choose_l_d << " - " <<  numsum_d << " / " << two_to_n_choose_l_d << " - " << denomsum_d << endl;
            long double p_onezc = (two_to_n_over_2_choose_l_d - numsum_d) / (two_to_n_choose_l_d - denomsum_d);
            //cout << "P(ZC"<<(col)<<"|!ZC[1->"<<col<<"]) = " << p_onezc << endl;

            pnot0 = pnot0 * (1.0-p_onezc);
        }

        p0 = 1.0 - pnot0;

        fout.precision(18);
        fout << l << "," << pnot0 << "," << p0 << endl;

        if (pnot0 == 1.0) {
            break;
        }
    }

    fout.close();

    return 0;
}
