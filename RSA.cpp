//
// Created by Andrew on 16-Apr-20.
//

#include <random>
#include <cassert>
#include <cstring>

#include <unordered_map>
#include <map>
#include <iostream>
#include "RSA.h"
#include "PrimeMiner.h"

using namespace std;

const uint RSA::ePool[] = {65537, 257, 17, 5};

void RSA::generateKeys(BigInt& N, BigInt& e, BigInt& d) {
    std::random_device dev;
    std::default_random_engine generator(dev());
    std::uniform_int_distribution<BigInt> distribution(UINT_MAX >> sizeof(UINT_MAX) * 8 * 5 / 8, // use 12
                                                       UINT_MAX >> sizeof(UINT_MAX) * 8 / 2); // use 16
    BigInt p = distribution(generator), q = distribution(generator);

    int testsPassed = 0;
    while (testsPassed < 64) {
        if (testRabinMiller(p)) {
            p++;
            testsPassed = 0;
        } else testsPassed++;
    }
    testsPassed = 0;

    while (testsPassed < 64) {
        if (testRabinMiller(q)) {
            q++;
            testsPassed = 0;
        } else testsPassed++;
    }

    N = p * q;
    BigInt phi = (p - 1) * (q - 1);
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;
    cout << "phi = " << phi << endl;


    if (!peekFromPool(e, phi)) {
        PrimeMiner& pm = PrimeMiner::getInstance();
        uint edge = 100;
        pm.fillPrimesUpTo(edge);
        set<BigInt>::iterator iter = pm.currPrimes().begin();
        ++iter; // skip 2
        ++iter; // skip 3
        ++iter; // skip 5
        set<BigInt>::iterator itrEnd = pm.currPrimes().end();
        // find an odd number e that is co-prime with phi
        while (phi % *iter == 0 || *iter % phi == 0) {
            ++iter;
            if (iter == itrEnd) {
                pm.fillPrimesUpTo(edge <<= 1u);
                iter = pm.currPrimes().begin();
                itrEnd = pm.currPrimes().end();
            }
        }
        e = *iter;
    }

    // e * d (mod phi) = 1
    d = 1ULL;
    while (d % e != 0)
        d += phi;

    d /= e;
    assert(d != 0 && d != phi);
    d %= phi;
    assert(e * d % phi == 1);
}

BigInt* RSA::encrypt(const BigInt& N, const BigInt& e, const char* message) {
    BigInt length = strlen(message);
    BigInt* ciphertext = new BigInt[length];
    bitset<sizeof(BigInt) * 8> encryptor(e);
    size_t lastBit = getLastBit(encryptor);
    unordered_map<char, BigInt> charCodes;
    for (BigInt i = 0; i < length; i++) {
        if (charCodes.find(message[i]) == charCodes.end()) {
            charCodes[message[i]] = powerMod(N, message[i], encryptor, lastBit);
        }
        ciphertext[i] = charCodes.find(message[i])->second;
    }
    return ciphertext;
}

char* RSA::decrypt(const BigInt& N, const BigInt& d, const BigInt* ciphertext, const size_t& length) {
    char* plainText = new char[length + 1];
    bitset<sizeof(BigInt) * 8> decryptor(d);
    size_t lastBit = getLastBit(decryptor);
    unordered_map<BigInt, char> charCodes;
    for (BigInt i = 0; i < length; i++) {
        if (charCodes.find(ciphertext[i]) == charCodes.end()) {
            charCodes[ciphertext[i]] = powerMod(N, ciphertext[i], decryptor, lastBit);
        }
        plainText[i] = charCodes.find(ciphertext[i])->second;
    }
    plainText[length] = '\0';
    return plainText;
}

BigInt gcd(BigInt a, BigInt b) {
    while (a != b)
        if (a > b) a -= b;
        else b -= a;
    return a;
}

bool RSA::peekFromPool(BigInt& e, const BigInt& phi) {
    for (uint i : ePool)
        if (gcd(phi, i) == 1) {
            e = i;
            return true;
        }

    return false;
}

BigInt
RSA::powerMod(const BigInt& N, const BigInt& base, const bitset<sizeof(BigInt) * 8>& exponent, const uint& lastBit) {
    BigInt res = exponent[0] ? base : 1;
    BigInt currValue = base;
    for (size_t i = 1; i <= lastBit; i++) {
        currValue = (currValue * currValue) % N;
        if (exponent[i]) {
            res *= currValue;
            res %= N;
        }
    }
    return res;
}

bool RSA::testRabinMiller(const BigInt& n) {
    const BigInt pred = n - 1;
    uint k = 0;
    while (pred % (1u << ++k) == 0);
    BigInt m = pred / 1u << --k;
    uint a = rand() % (pred - 2) + 2;

    bitset<sizeof(BigInt) * 8> bsm(m);
    BigInt b0 = powerMod(n, a, bsm, getLastBit(bsm));
    if (b0 == 1 || b0 == pred) return false;
    BigInt b1 = (b0 * b0) % n;
    for (int i = 0; i < 10; i++)
        if (b1 == 1) return true;
        else if (b1 == pred) return false;
    return true; // if no useful results provided, assume it is not prime
}

uint RSA::getLastBit(const std::bitset<sizeof(BigInt) * 8>& exponent) {
    size_t lastBit = 0;
    for (size_t i = 0; i < exponent.size(); i++) {
        if (exponent[i]) lastBit = i;
    }
    return lastBit;
}
