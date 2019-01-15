/*
 * Compute binomials (N m) aka "N choose m". N is selected as the
 * number of bits in a genome large enough to specify transformation
 * rules for nGenes genes, which defaults to 5, but can be modified
 * with a single argument to this program.
 *
 * Author Seb James, Oct 2018
 */

#include <iostream>
#include "lmp.h"
#include "xmath.h"

using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;

#define N_Genes 5

int main (int argc, char** argv)
{
    ulong nGenes = N_Genes;
    if (argc > 1) {
        nGenes = (ulong)atoi (argv[1]);
    }
    cerr << "nGenes: " << nGenes << endl;

    // Compute N choose m and output to stdout
    ulong N = nGenes * (1<<(nGenes-1));
    cerr << "N bits: " << N << endl;

    Xint N_choose_m;
    lmp::InitSetUi(N_choose_m, 1);
    for (ulong m = 0; m <= N; ++m) {
        lmp::BinomialUiUi (N_choose_m, N, m);
        cout << N_choose_m << endl;
    }
    lmp::Clear(N_choose_m);

    return 0;
}
