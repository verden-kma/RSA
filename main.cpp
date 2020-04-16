#include <iostream>
#include "PrimeMiner.h"

using namespace std;

int main() {
    PrimeMiner pm = PrimeMiner::getInstance();
    pm.fillPrimesUpTo(100);
    pm.fillPrimesUpTo(1000);
    cout << pm.currPrimes().size() << endl;
    set<BigInt>::iterator iter = pm.currPrimes().begin();
    while(iter != pm.currPrimes().end()) {
        cout << *iter << endl;
        ++iter;
    }
}