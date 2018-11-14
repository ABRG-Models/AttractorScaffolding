/*
 * Show the fitness of a genome
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>

#include "modelfn.h"
#include "net.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc<2) {
        cout << "usage: " << argv[0] << " genome"
             << endl << "e.g. : " << argv[0] << " 01001001...." << endl;
        return 0;
    }

    int nGenes = 5;
    Net N(nGenes);
    string gstring(argv[1]);
    vector<bool> gtrial = str2genome (gstring, (unsigned int)nGenes);
    N.G = gtrial;
    double f = N.fitness();
    cout << gstring << " has fitness " << f << endl;

    return 0;
}
