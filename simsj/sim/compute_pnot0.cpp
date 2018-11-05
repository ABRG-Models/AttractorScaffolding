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

double
xint_to_double (Xint& xi)
{
    double d;
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
    int n = (1<<ngenes);   // 2^ngenes

    for (int l = 1; l<=(1<<(ngenes-1)); ++l) {

        Xint two_to_n_choose_l;
        Xint two_to_n_over_2_choose_l;
        lmp::InitSetUi (two_to_n_choose_l, 1);
        lmp::InitSetUi (two_to_n_over_2_choose_l, 1);

        double two_to_n_choose_l_d = 0.0;
        double two_to_n_over_2_choose_l_d = 0.0;
        double two_to_n_minus_1_over_2i_choose_l_d = 0.0;
        double two_to_n_over_2i_choose_l_d = 0.0;
        double m_choose_i_d = 0.0;
        double numsum_d = 0.0;
        double denomsum_d = 0.0;

        double P0 = 0.0;
        double PNOT = 1.0;

        // Once only:
        lmp::BinomialUiUi(two_to_n_over_2_choose_l, 1<<(ngenes-1), l);
        two_to_n_over_2_choose_l_d = xint_to_double (two_to_n_over_2_choose_l);

        lmp::BinomialUiUi(two_to_n_choose_l, 1<<ngenes, l);
        two_to_n_choose_l_d = xint_to_double (two_to_n_choose_l);

        double col0 = two_to_n_over_2_choose_l_d / two_to_n_choose_l_d;
        PNOT = 1.0-col0;
        //cout << "P(ZC0) = " << col0 << endl;
        //cout << "P(!ZC0) = " << PNOT << endl;

        for (ulong col = 1; col < ngenes; ++col) {
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
            double dd = (two_to_n_over_2_choose_l_d - numsum_d) / (two_to_n_choose_l_d - denomsum_d);
            //cout << "P(ZC"<<(col)<<"|!ZC[1->"<<col<<"]) = " << dd << endl;

            PNOT = PNOT * (1.0-dd);
        }

        cout.precision(18);
        cout << l << "," << PNOT << endl;
    }
    return 0;
}
