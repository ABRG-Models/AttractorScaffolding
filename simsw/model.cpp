/*
 * Implementation of a Boolean network model of the evolution of
 * cortical development as described by Wilson, James, Whiteley, and
 * Krubitzer
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>

#include "modelfn.h"
#include "net.h"

using namespace std;

/*!
 * For mixing up bits of three args; used to generate a good random
 * seed using time() getpid() and clock().
 */
unsigned int
mix (unsigned int a, unsigned int b, unsigned int c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

// MAIN EVOLUTIONARY LOOP
int main(int argc, char **argv){

    if(argc<6){
        cout<<"useage: ./model filename genes mutation seed generations"<<endl<<"e.g.  : ./model output 5 0.05 1"<<endl;
        return 0;
    }
    // ./model file Ngenes Prob Seed nGens

    // Seed the RNG.
    unsigned int seedy = 1;//mix(clock(), time(NULL), getpid());
    srand (seedy);

    ofstream outFile; std::stringstream oFile; oFile<<argv[1]<<".bin";
    outFile.open(oFile.str().c_str(),ios::out|ios::trunc|ios::binary);
    if (outFile.is_open() == false) {
        cerr << "Error opening file " << oFile.str() << ", exiting." << endl;
        return 1;
    }
    ofstream fout; // For csv data
    oFile.str(""); oFile.clear();
    oFile << argv[1] << ".csv";
    fout.open (oFile.str().c_str(),ios::out|ios::trunc);
    if (fout.is_open() == false) {
        cerr << "Error opening file " << oFile.str() << ", exiting." << endl;
        return 1;
    }

    int nGenes = atoi(argv[2]);
    double mutationProbability = atof(argv[3]);
    //int seed = atoi(argv[4]); // unused
    int nGenerations = atoi(argv[5]);

    Net N(nGenes);

#if 0 // For testing of fitness
    string gstring = "00111111011101010111010110001111101110101010000101011100111000101011110000101011";
    vector<bool> gtrial = str2genome (gstring, (unsigned int)nGenes);
    N.G = gtrial;
    cout << gstring << " has fitness " << N.fitness() << endl;
    cout << "FIRST FITNESS DONE" << endl;

    gstring = "11011111010000111110000100110110011000000011001100011010101010000111011101011010";
    gtrial = str2genome (gstring, (unsigned int)nGenes);
    N.G = gtrial;
    cout << gstring << " has fitness " << N.fitness() << endl;
#endif

    int interval = 0;
    double fa = 0.;

    int gens25 = nGenerations/4;
    int gens50 = gens25*2;
    int gens75 = gens25*3;

    unsigned int numzeros = 0;
    unsigned int numones = 0;

    for (int t=0;t<nGenerations;t++){

#if 0
        if(t==gens25) cout<<endl<<" 25% "<<endl;
        if(t==gens50) cout<<endl<<" 50% "<<endl;
        if(t==gens75) cout<<endl<<" 75% "<<endl;
#endif

        if (fa == 1.0){

            //cout<<"."<<flush;
            cout<<"FIT 1"<<endl;

            outFile.write((char*)(&interval),sizeof(int));
            fout << interval << endl;
            interval = 0;
            N.randomGenome();
            cout << "fa: ";
            fa = N.fitness();
            numones++;

        } else {
            vector<bool> Gpre = N.G;
            for(int i=0;i<Gpre.size();i++){
                if(randBool(mutationProbability)) {
                    N.G[i] = !N.G[i];
                }
            }
            cout << "fb: ";
            double fb = N.fitness();

            if (fa == 0.0) { numzeros++; }
#ifdef DRIFT
#warning 'Compiling with drift'
            if(fb<fa) {
                N.G=Gpre; // discard N.G; revert to Gpre
            } else if (fb==fa) { //  DRIFT
                cout << "fb==fa=" << fb << endl;
                fa=fb; // and keep N.G
            } else { // keep N.G as fb is > fa
                cout << "fb>fa, so fb=fa=" << fb << endl;
                fa=fb;
            }
#else
#warning 'Compiling with NO drift'
            if(fb<fa) {
                N.G=Gpre;
            } else if (fb==fa) { // NO DRIFT. Equal, but reject new one.
                cout << "fb==fa=" << fa << " copy Gpre back into N.G..." << endl;
                N.G=Gpre;
            } else { // keep N.G as fb is > fa
                cout << "fb>fa so fb=fa=" << fa << endl;
                fa=fb;
            }
#endif

            interval++;
        }
    }
    outFile.close();
    fout.close();

    cout << "Zero fitness occurred " << numzeros << " times out of " << nGenerations << endl;
    cout << "Full fitness occurred " << numones << " times" << endl;
    return 0;
}
