/*
 * Examine bit-flip statistics
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
    if (argc<6) {
        cout << "usage: " << argv[0] << " filename genes mutation seed generations"
             << endl << "e.g. : " << argv[0] << " output 5 0.05 1" << endl;
        return 0;
    }
    // ./model file Ngenes Prob Seed nGens

    ofstream outFile; std::stringstream oFile; oFile<<argv[1]<<".bin";
    outFile.open(oFile.str().c_str(),ios::out|ios::binary);

    if (outFile.is_open() == false) {
        cerr << "Error opening file " << oFile.str() << ", exiting." << endl;
        return 1;
    }

    int nGenes = atoi(argv[2]);
    double mutationProbability = atof(argv[3]);
    int seed = atoi(argv[4]);
    int nGenerations = atoi(argv[5]);

    Net N(nGenes);

    int gens25 = nGenerations/4;
    int gens50 = gens25*2;
    int gens75 = gens25*3;

    unsigned long long int numFlips = 0;
    int sz = N.G.size();
    vector<unsigned long long int> bitcounts(sz, 0);

    for (int t=0;t<nGenerations;t++){

        if(t==gens25) cout<<endl<<" 25% "<<endl;
        if(t==gens50) cout<<endl<<" 50% "<<endl;
        if(t==gens75) cout<<endl<<" 75% "<<endl;

        for(int i=0;i<sz;i++){
            if(randBool(mutationProbability)) {
                N.G[i] = !N.G[i];
                ++numFlips;
                ++bitcounts[i];
            }
        }
    }

    cout << "Mean number of flips for pOn=" << mutationProbability << " is "
         << numFlips / (double)nGenerations << endl;

    for(int i=0;i<sz;i++){
        cout << "mean flips for bit " << i << ":" << bitcounts[i]/(double)nGenerations << endl;
    }

    return 0;
}
