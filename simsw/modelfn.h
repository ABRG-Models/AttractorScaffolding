#ifndef _MODELFN_H_
#define _MODELFN_H_

#include <vector>
#include <string>

using namespace std;

vector<vector<int> > graph(vector<vector<int> >);
double randDouble(void);
bool randBool(double);
int bin2int(vector<bool>);
vector<bool> int2bin(long int, long int);
vector<bool> str2genome (const string& s, unsigned int ngenes);

#endif // _MODELFN_H_
