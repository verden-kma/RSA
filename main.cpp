#include <iostream>
#include "PrimeMiner.h"
#include "RSA.h"
#include <bitset>

using namespace std;

int main() {
//    PrimeMiner& pm = PrimeMiner::getInstance();
//    pm.fillPrimesUpTo(100);
//    pm.fillPrimesUpTo(1000);
//    cout << pm.currPrimes().size() << endl;
//    set<BigInt>::iterator iter = pm.currPrimes().begin();
//    while(iter != pm.currPrimes().end()) {
//        cout << *iter << endl;
//        ++iter;
//    }

//    cout << RSA::logPow(3, 9) << endl;

//    BigInt N, e, d;
//    RSA::generateKeys(N, e, d);
//    cout << N << endl;
//    cout << e << endl;
//    cout << d << endl;
    unsigned long long exp = 16971;
    bitset< sizeof(exp)> ebs (exp);
    cout << RSA::powerMod(25777, 18524, ebs, ebs.size()) << endl;
}