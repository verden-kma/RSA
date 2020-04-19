//
// Created by Andrew on 16-Apr-20.
//

#ifndef PROJECTOOP_RSA_H
#define PROJECTOOP_RSA_H

#include <bitset>

using BigInt = unsigned long long;
using uint = unsigned int;

class RSA {
private:
    static const uint ePool[];
    static bool peekFromPool(BigInt& e, const BigInt& phi);
public:

    static BigInt powerMod(const BigInt& N, const BigInt& base, const std::bitset<sizeof(BigInt)*8>& exponent, const uint& lastSet);

    RSA() = delete;
    static void generateKeys(BigInt& N, BigInt& e, BigInt& d);
    static BigInt* encrypt(const BigInt& N, const BigInt& e, const char* message);
    static const char* decrypt(const BigInt& N, const BigInt& d, const char* ciphertext);
};


#endif //PROJECTOOP_RSA_H
