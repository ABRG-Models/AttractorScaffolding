#include <iostream>
#include <sstream>

#define TRY_PRIMESWING 1

#include "lmp.h"
#ifdef TRY_PRIMESWING
# include "xmath.h"
# include "primeswing.h"
#endif

using std::cout;
using std::endl;
using std::stringstream;

#define N 192000

int main()
{
#ifdef TRY_PRIMESWING
    Xint xi;
    lmp::InitSetUi(xi, 1);
    ulong n = 15;
    PrimeSwing::ParallelFactorial (xi, n);
    cout << n << "!: " << xi << endl;
    lmp::Clear(xi); // Actually seems to free it up.

    n = N;
#else
    ulong n = N;
#endif

    // n choose k: n! / (k!(n-k)!)

    ulong k = 78;

    Xint nchoosek;
    lmp::InitSetUi(nchoosek, 1);

    Xint xn, xk, xnk, denom;
    lmp::InitSetUi(xn, 1);
    lmp::InitSetUi(xk, 1);
    lmp::InitSetUi(xnk, 1);
    lmp::InitSetUi(denom, 1);

#ifdef TRY_PRIMESWING
    PrimeSwing::ParallelFactorial (xn, n);
    PrimeSwing::ParallelFactorial (xk, k);
    PrimeSwing::ParallelFactorial (xnk, (n-k));
    // xn / (xk * xnk)
    lmp::Mul(denom, xk, xnk);
    lmp::Div(nchoosek, xn, denom);
    cout << n << " choose " << k << ":" << nchoosek << endl;
#endif

    lmp::BinomialUiUi(nchoosek, n, k);
    cout << n << " choose " << k << " by 2nd method:" << nchoosek << endl;

    double nckdouble = 0.0;
    stringstream ss;
    ss << nchoosek;
    ss >> nckdouble;
    cout << n << " choose " << k << " by 2nd method cast to double:" << nckdouble << endl;

    return 0;
}
