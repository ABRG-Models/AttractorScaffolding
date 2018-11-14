/*
 * Create 1000 genomes, save their strings to file and their fitnesses to file.
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
    int nGenes = 5;
    Net N(nGenes);

    ofstream gout, fout;
    gout.open ("./thousand_genomes.csv", ios::out|ios::trunc);
    fout.open ("./thousand_fitness.bin", ios::out|ios::trunc|ios::binary);

    for (unsigned int i = 0; i < 1000; ++i) {
        N.randomGenome();
        double f = N.fitness();
        gout << N.Gstring() << endl;
        fout.write((char*)(&f),sizeof(double));
    }

    gout.close();
    fout.close();

    return 0;
}
