// Compute n choose k: n! / (k!(n-k)!)

#include <iostream>
#include <sstream>

#include "lmp.h"

using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;

#define N 192000

int main(int argc, char** argv)
{
    if (argc < 3) {
        cerr << "usage: " << argv[0] << " n k" << endl;
        return 1;
    }

    ulong n = (ulong)atoi(argv[1]);
    ulong k = (ulong)atoi(argv[2]);

    Xint nchoosek;
    lmp::InitSetUi(nchoosek, 1);

    lmp::BinomialUiUi(nchoosek, n, k);
    cout << n << " choose " << k << ": " << nchoosek << endl;

    long double nckdouble = 0.0;
    stringstream ss;
    ss << nchoosek;
    ss >> nckdouble;
    cout << n << " choose " << k << " (long double): " << nckdouble << endl;

    lmp::Clear(nchoosek);

    return 0;
}
