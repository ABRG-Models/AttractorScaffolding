// By computing n choose k: n! / (k!(n-k)!) many times, display Pascal's triangle values.

#include <iostream>
#include <sstream>

#include "lmp.h"

using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;

int main(int argc, char** argv)
{
    if (argc < 2) {
        cerr << "usage: " << argv[0] << " N (N is number of rows in Pascal's triangle to display" << endl;
        return 1;
    }

    ulong N = (ulong)atoi(argv[1]);

    Xint nchoosek;
    lmp::InitSetUi(nchoosek, 1);

    for (ulong n = 0; n < N; ++n) {
        for (ulong k = 0; k<n+1; ++k) {
            lmp::BinomialUiUi (nchoosek, n, k);
            cout << nchoosek << ",";
        }
        cout << endl;
    }

    lmp::Clear(nchoosek);

    return 0;
}
