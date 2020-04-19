#include <iostream>
#include "PrimeMiner.h"
#include "RSA.h"
#include <bitset>
#include <cstring>

using namespace std;

int power(const int base, const int exponent, const int mod) {
    int res = 1;
    for (int i = 0; i < exponent; i++) {
        res *= base;
        res %= mod;
    }
    return res;
}

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

    BigInt N, e, d;
    RSA::generateKeys(N, e, d);
    cout << "N = " << N << endl;
    cout << "e = " << e << endl;
    cout << "d = " << d << endl;

    string secret = "my secret";
    char arrSecret[secret.length()];
    strcpy(arrSecret, secret.c_str());
    auto c = RSA::encrypt(N, e, arrSecret);
    cout << "E: " << *c << endl;
    char* m = RSA::decrypt(N, d, c, secret.length());
    cout << m << endl;

//    BigInt N = 538296929;
//    BigInt base = 65;
//    BigInt exp = 65537;
//    bitset<64> expBS(exp);
//    cout << RSA::powerMod(N, base, expBS, RSA::getLastBit(expBS)) << endl;

//BigInt phi = (1905598061 - 1) * (3590304131 - 1);
//BigInt e = 65537;
//BigInt q = 2780153;
//
//    //cout << (17 * 2753 % (60 * 52) == 1) << endl;
//    cout << (e * q % (phi) == 1) << endl;

//    for (int i = 0; i < 1; i++)
//        cout << "is prime: " << !RSA::testRabinMiller(3590304121) << endl;

}