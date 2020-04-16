//
// Created by Andrew on 16-Apr-20.
//

#include <random>
#include <cassert>
#include <cstring>
#include <bitset>
#include <unordered_map>
#include <map>
#include "RSA.h"
#include "PrimeMiner.h"

using namespace std;

const uint RSA::ePool[] = {5, 17, 257, 65537};

void RSA::generateKeys(BigInt& N, BigInt& e, BigInt& d) {
    std::random_device dev;
    std::default_random_engine generator(dev());
    std::uniform_int_distribution<unsigned int> distribution(UINT_MAX / 10, UINT_MAX);
    unsigned int p = distribution(generator);
    unsigned int q = distribution(generator);
    N = p * q;
    BigInt phi = (p - 1) * (q - 1);

    if (!peekFromPool(e, phi)) {
        PrimeMiner& pm = PrimeMiner::getInstance();
        unsigned int edge = 100;
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
    while (d % e != 0) {
        d += phi;
    }
    d /= e;
    assert(d != 0 && d != phi);
    while (d > phi) d -= phi;
}

double logPower(double x, unsigned int n) {
    double y = 1;
    while (n > 0)
        if (n & 0x1u) {
            n--;
            y *= x;
        } else {
            n >>= 0x1u;
            x *= x;
        }
    return x;
}

BigInt* RSA::encrypt(const BigInt& N, const BigInt& e, const char* message) {
    BigInt length = strlen(message);
    BigInt* ciphertext = new BigInt[length];
    bitset<sizeof(BigInt)> exponent(e);
    size_t lastBit = 0;
    // get last set bit so that there is no need to iterate through leftmost 0s
    for (size_t i = 0; i < exponent.size(); i++) {
        if (exponent[i]) lastBit = i;
    }
    unordered_map<const char, BigInt> charCodes; // cache
    for (BigInt i = 0; i < length; i++) {
        // check if charCodes does not contain this char
        map<uint, BigInt> powers; // power of 2 - value of number to the power of power of 2
        for (size_t j = 0; j < lastBit; j++) {
            if (exponent[j]) {
// if powers contains any values, use the biggest cache for as many times as possible, then move on to small cached value,
// go on until cache is exhausted, then use logarithmic power
            }
        }
        charCodes.insert(message[i], -1);
    }
    return ciphertext;
}

bool RSA::peekFromPool(BigInt& e, const BigInt& phi) {
    for (int i = 0; i < sizeof(ePool) / sizeof(*ePool); i++)
        if (phi % ePool[i] != 0 && ePool[i] % phi != 0) {
            e = ePool[i];
            return true;
        }

    return false;
}
