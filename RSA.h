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

    static bool testRabinMiller(const BigInt& n);

    /**
 * get last set bit so that there is no need to iterate through leftmost 0s
 * @param bitset representation of exponent
 * @return last set bit
 */
    static uint getLastBit(const std::bitset<sizeof(BigInt) * 8>& exponent);

    static BigInt powerMod(const BigInt& N, const BigInt& base, const std::bitset<sizeof(BigInt) * 8>& exponent, const uint& lastSet);

public:
    RSA() = delete;

    static void generateKeys(BigInt& N, BigInt& e, BigInt& d);

    static BigInt* encrypt(const BigInt& N, const BigInt& e, const char* message);

    static char* decrypt(const BigInt& N, const BigInt& d, const BigInt* ciphertext, const size_t& length);
};


#endif //PROJECTOOP_RSA_H
