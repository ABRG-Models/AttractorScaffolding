/*
 * Compute a!/b! with a, b given on command line.
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
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " a b" << endl;
        return 1;
    }
    ulong a = (ulong)atoi (argv[1]);
    ulong b = (ulong)atoi (argv[2]);

    Xint afac, bfac, res;
    lmp::InitSetUi(afac, 1);
    lmp::InitSetUi(bfac, 1);
    lmp::InitSetUi(res, 1);
    lmp::FacUi (afac, a);
    lmp::FacUi (bfac, b);
    lmp::Div (res, afac, bfac);
    cout << a << "!/" << b << "! = " << res << endl;
    lmp::Clear(afac);
    lmp::Clear(bfac);
    lmp::Clear(res);

    return 0;
}
