#include "quine.h"

using namespace std;

int main()
{
    int rtn = 0;

    int ngenes = 3;
    Quine Q(ngenes);
    Q.addMinterm(0);
    Q.addMinterm(1);
    Q.addMinterm(3);
    Q.addMinterm(4);
    Q.addMinterm(5);
    Q.addMinterm(7);
    Q.go();
    cout << "Complexity: " << Q.complexity() << endl;
    cout << "Minimal expression: " << Q.min() << endl;;

    if (Q.complexity() != 0.25) {
        rtn = -1;
    }

    return rtn;
}
