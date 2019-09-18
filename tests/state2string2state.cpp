/*
 * Tests the state to string format conversion
 *
 * Author: S James
 * Date: September 2019.
 */

#include <iostream>
#include <vector>
#include <set>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

// Choose debugging level.
//
#define DEBUG 1

// Number of genes in a state is set at compile time.
#define N_Genes 5

// Common code
#include "lib.h"

int testconversions (const string& str)
{
    int rtn = 0;
    // Convert to state_t
    state_t s1 = str2state (str);
    cout << "\ns1=" << str << " or " << (unsigned int)s1 << " decimal" << endl;
    // Convert back to string (will have spaces)
    string s2s = state_str (s1);
    cout << "after conversion back to string, it's: " << s2s << endl;
    // Convert to state_t again
    state_t s2 = str2state (s2s);
    // Test
    if (s1 != s2) {
        cout << "Test failed." << endl;
        rtn -= 1;
    }
    return rtn;
}

int main (int argc, char** argv)
{
    // Initialise masks
    masks_init();

    int rtn = 0;

    // Original string
    string s1s = "10000";
    rtn += testconversions (s1s);

    s1s = "10101";
    rtn += testconversions (s1s);

    s1s = "01010";
    rtn += testconversions (s1s);

    return rtn;
}
