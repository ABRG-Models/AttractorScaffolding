/*
 * Compute factorial of number given on command line.
 *
 * Author Seb James, Oct 2018
 */

#include <iostream>
#include "lmp.h"
#include "xmath.h"

using std::cout;
using std::cerr;
using std::endl;

int main (int argc, char** argv)
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " n" << endl;
        return 1;
    }
    ulong n = (ulong)atoi (argv[1]);

    Xint nfac;
    lmp::InitSetUi(nfac, 1);
    lmp::FacUi (nfac, n);
    cout << n << "! = " << nfac << endl;
    lmp::Clear(nfac);

    return 0;
}
