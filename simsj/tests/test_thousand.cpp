/*
 * Tests 1000 genomes in thousand_genomes.csv and compares results
 * with thousand_fitnesses.bin.
 *
 * Author: S James
 * Date: November 2018.
 */

#include <iostream>
#include <vector>
#include <set>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>

using namespace std;

// Choose debugging level.
//
//#define DEBUG 1
// #define DEBUG2 1

// Number of genes in a state is set at compile time.
#define N_Genes 5

// Common code
#include "lib.h"

#include "fitness.h"

int main (int argc, char** argv)
{
    // Initialise masks
    masks_init();

    ifstream gin, fin;

    gin.open ("../tests/thousand_genomes.csv", ios::in);
    fin.open ("../tests/thousand_fitness.bin", ios::in|ios::binary);

    if (fin.is_open() == false || gin.is_open() == false) {
        gin.open ("../../tests/thousand_genomes.csv", ios::in);
        fin.open ("../../tests/thousand_fitness.bin", ios::in|ios::binary);
        if (fin.is_open() == false || gin.is_open() == false) {
            cerr << "Failed opening data files." << endl;
            return 1;
        }
    }

    unsigned int l_genosect = 1 << N_Ins;
    unsigned int l_genome = N_Genes * l_genosect;

    string s("");
    unsigned int count = 0;
    double max_fdiff = 0.0;
    while (getline (gin, s)) {

        unsigned int l = s.length();
        if (l == l_genome) {
            //LOG ("String has " << l_genome << " bit chars as required...");
        } else {
            LOG ("String does not have " << l_genome << " bit chars as required. Exiting.");
            return 1;
        }

        array<genosect_t, N_Genes> g = str2genome (s);
        double f = evaluate_fitness (g);
        double fexpt = 0.0;
        fin.read((char*)&fexpt, sizeof(double));
        cout.precision(12);
        cout << "Computed fitness: " << f << " and expected (from file): " << fexpt << endl;
        double fdiff = abs(f - fexpt);
        if (fdiff > max_fdiff) { max_fdiff = fdiff; }
        cout << "Fitness difference: " << fdiff << endl;
        if (fdiff > 0.000000000000001) {
            cerr << "Failed on genome number " << count << endl;
            return 1;
        }
        ++count;
    }
    cout << "Max fitness difference was " << max_fdiff << endl;
    return 0;
}
