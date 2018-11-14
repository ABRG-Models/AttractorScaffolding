/*
 FUNCTIONS
 */
#include "modelfn.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

// RETURN A RANDOM DOUBLE BETWEEN 0 AND 1
double randDouble(void){
    return ((double) rand())/(double)RAND_MAX;
}

// RETURN A BOOL WITH PROBABILITY OF BEING TRUE GIVEN BY P
bool randBool(double P){
    return (randDouble()<P);
}

// CONVERT A BINARY (VECTOR OF BOOLS) TO AN INTEGER
int bin2int(vector<bool> b){
    int k = 0;
    int s = 1;
    for(int i=b.size();i--;){
        k += s*b[i];
        s *= 2;
    }
    return k;
}

// CONVERT AN INTEGER TO A BINARY (VECTOR OF BOOLS)
vector<bool> int2bin(long int i, long int n){
    vector<bool> x(n,false);
    long int a = pow(2,n-1);
    for(long int k=0;k<n;k++){
        x[k] = (i>=a);
        if(x[k]) i-=a;
        a/=2;
    }
    return x;
}


// TAKES A LIST OF PAIRS AND RETURNS A VECTOR OF CLUSTERS SORTED BY CLUSTER SIZE
vector<vector<int> > graph(vector<vector<int> > conn){
    int N = conn.size();
    vector<vector<int> > graph;
    int a, b, Ain, Bin;
    bool nov, ain, bin;
    for(int k=0;k<conn.size();k++){
        a = conn[k][0];
        b = conn[k][1];
        if(a != b){
            // reset flags
            Ain = -1;
            Bin = -1;
            nov = true;
            // check if conn k is in the graph
            int I = graph.size();
            for(int i=0;i<I;i++){
                ain = false;
                bin = false;
                int J = graph[i].size();
                for(int j=0;j<J;j++){
                    if(a == graph[i][j]){
                        ain = true;
                        Ain = i;
                    }
                    if(b == graph[i][j]){
                        bin = true;
                        Bin = i;
                    }
                }
                if(ain && !bin) graph[i].push_back(b);
                if(!ain && bin) graph[i].push_back(a);
                if(ain||bin) nov=false;
            }
            // Add a new group
            if(nov) graph.push_back(conn[k]);

            // Join two existing groups
            if(Ain>-1 && Bin>-1 && Ain!=Bin){
                graph[Ain].pop_back();
                graph[Bin].pop_back();
                for(int l=0;l<graph[Bin].size();l++){
                    graph[Ain].push_back(graph[Bin][l]);
                }
                graph.erase(graph.begin()+Bin);
            }
        }
    }

    for(int k=0;k<N;k++){
        bool isolated = true;
        int I = graph.size();
        for(int i=0;i<I;i++){
            int J = graph[i].size();
            for(int j=0;j<J;j++){
                if(k==graph[i][j]){
                    isolated = false;
                    //break;
                }
            }
        }
        if(isolated){
            vector<int> isolate(1,k);
            graph.push_back(isolate);
        }
    }

    return graph;
}

vector<bool>
str2genome (const string& s, unsigned int ngenes)
{
    vector<bool> g;
    // Init g
    unsigned int nbits = ngenes * (1<<(ngenes-1));
    for (unsigned int i = 0; i < nbits; ++i) {
        g.push_back(false);
    }

    // Check length of string.
    unsigned int l = s.length();
    if (l == nbits) {
        // Ok
    } else {
        return g;
    }

    for (unsigned int i = 0; i < nbits; ++i) {
        bool high = (s[i] == '1');
        g[i] = high;
        cout << (g[i]==true?'1':'0');
    }
    cout << endl;

    return g;
}
