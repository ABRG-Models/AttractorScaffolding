#ifndef _NET_H_
#define _NET_H_

#include <vector>
#include <math.h>

#include "modelfn.h"

using namespace std;

/*
 DEFINE BOOLEAN NETWORK
 */

class Net {

public:
    long int nG, nS, nT;
    vector<bool> G;                 // genome
    vector<vector<int> > A, C;      // attractors, connections
    vector<int> I;                  // attractor ID
    double normVal;
    vector<long int> targets, initials;
    double minF;

    Net(long int nG){

        this->nG = nG;          // number of genes
        nS = pow(2,nG);         // number of states
        nT = pow(2,nG-1);       // number of truth table combinations
        normVal = 1./(pow(2,nG+1)); // for fitness function

        C.resize(nS);
        for(int s=0;s<nS;s++){ C[s].resize(2); }

        G.resize(nG*nT);
        I.resize(nS);

        // define initial and target states
        vector<bool> dummy(nG,false);
        dummy[0]=0;
        dummy[1]=0;
        dummy[2]=0;
        dummy[3]=0;
        dummy[4]=0;
        dummy[5]=0;
        dummy[6]=0;
        dummy[7]=1;
        initials.push_back(bin2int(dummy));

        dummy[0]=1;
        dummy[1]=0;
        dummy[2]=0;
        dummy[3]=0;
        dummy[4]=0;
        dummy[5]=0;
        dummy[6]=0;
        dummy[7]=1;
        initials.push_back(bin2int(dummy));

        dummy[0]=0;
        dummy[1]=1;
        dummy[2]=0;
        dummy[3]=1;
        dummy[4]=0;
        dummy[5]=0;
        dummy[6]=0;
        dummy[7]=0;
        targets.push_back(bin2int(dummy));

        dummy[0]=1;
        dummy[1]=0;
        dummy[2]=1;
        dummy[3]=0;
        dummy[4]=1;
        dummy[5]=0;
        dummy[6]=0;
        dummy[7]=1;
        targets.push_back(bin2int(dummy));

        randomGenome();

        minF = 0.0000001;
    }

    // generate a random genome
    void randomGenome(void){
        for(long int i=0;i<nG*nT;i++){
            G[i]=randBool(0.5);
        }
    }

    vector<bool> getNext(vector<bool> S){
        vector<bool> Snext(nG,false);
        for(long int i=0;i<nG;i++){
            int k = 0;
            int s = 1;
            for(int j=nG;j--;){
                if(i!=j){
                    k+=s*S[j];
                    s*=2;
                }
            }
            Snext[i] = G[i*nT+k];
        }
#if 0
        for (unsigned int i = 0; i < nG; ++i) {
            cout << Snext[i]?'1':'0';
        }
        cout << endl;
#endif
        return Snext;
    }

    // return number of iterations before a state is repeated
    int getDist(long int s){
        int d = 0;
        vector<bool> V (nS,false); // visited states
        vector<bool> S = int2bin(s,nG);
        while(true){
            int v = bin2int(S);
            if(V[v]) break;
            V[v] = true;
            d++;
            S = getNext(S);
        }
        return d-1;
    }


    // return number of iterations before a state is repeated
    double getCycle(long int s, long int tar){

        cout << "getCycle for start: " << s << endl;
        vector<bool> V (nS,false); // visited states
        vector<bool> S = int2bin(s,nG);
        long int v;
        while (true){
            v = bin2int(S);
            cout << v << " --> ";
            if(V[v]) break;
            V[v] = true;
            S = getNext(S);
        }
        cout << endl;

        S = int2bin(v,nG);     // reset to limit cycle state

        // Go round for another turn to count limit cycle length
        int d = 0;
        while(true){
            d++;
            S = getNext(S);
            if (bin2int(S) == v) {
                break;
            }
        }
        cout << "d is " << d << endl;

        vector<bool> F = int2bin(tar,nG);   // target state for comparison
        vector<double> fit(nG,0.);
        double ft = 1.0;

        for (int i=0;i<d;i++){
            for(int j=0;j<nG;j++){
                fit[j] += fabs((double)S[j]-(double)F[j]);
            }
#if 0
            for (unsigned int ii = 0; ii < nG; ++ii) {
                cout << S[ii]?'1':'0';
            }
            cout << endl;
#endif
            S = getNext(S);
        }
        //cout << "-----" << endl;
        for(int j=0;j<nG;j++){
            ft *= 1.-fit[j]/(double)d;
            //ft *= 1.-fmax(fit[j]/(double)d,minF);
        }

        return ft;
    }

    // return whether there is a path from a to b
    bool isPath(long int start, long int finish){
        vector<bool> S = int2bin(start,nG);
        for(int i=0;i<=nS;i++){
            S = getNext(S);
            if (bin2int(S)==finish) return true;
        } return false;
    }


    // compute the attractor landscape
    void getAttractors(void) {

        for(int s=0;s<nS;s++){
            C[s][0] = s;
            C[s][1] = bin2int(getNext(int2bin(s,nG)));
        }
        A = graph(C);

        for(int i=0;i<A.size();i++){
            for(int j=0;j<A[i].size();j++){
                I[A[i][j]] = i;
            }
        }

        cout << "graph: " << endl;
        for(int i=0;i<A.size();i++){
            cout << A[i][0];
            for(int j=1;j<A[i].size();j++){
                cout << " --> " << A[i][j];
            }
            cout << endl;
        }
    }

    double fitness(void){ // don't force on separate attractors
        double f = 1.;
#if 0
        for (int i=0;i<G.size();i++){
            cout << (G[i]==true?'1':'0');
        }
#endif
        for (int i=0;i<targets.size();i++){
            double d = getCycle(initials[i],targets[i]);
            cout << "fitness part " << i << "=" << d << endl;
            f *= d;
        }
#if 0
        cout << ", fitness: " << f << endl;
#endif
        return f;
    }

    void showG (void) {
        for (unsigned int i = 0; i<G.size(); ++i) {
            cout << (G[i]==true ? '1':'0');
        }
        cout << endl;
    }

    string Gstring (void) {
        stringstream ss;
        for (unsigned int i = 0; i<G.size(); ++i) {
            ss << (G[i]==true ? '1':'0');
        }
        return ss.str();
    }
};

#endif // _NET_H_
