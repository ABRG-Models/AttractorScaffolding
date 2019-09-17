#include "quin.h"

using namespace std;

int main()
{
    int ngenes = 3;
    Quin Q(1, ngenes);
    Q.addMinterm(0);
    Q.addMinterm(1);
    Q.addMinterm(3);
    Q.addMinterm(4);
    Q.addMinterm(5);
    Q.addMinterm(7);
    Q.go();
    cout << "Complexity: " << Q.complexity() << "/" << Q.outof << endl;
    cout << "Minimal expression: " << Q.min() << endl;;
}
