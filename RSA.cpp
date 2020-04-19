//
// Created by Andrew on 16-Apr-20.
//

#include <random>
#include <cassert>
#include <cstring>

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

BigInt* RSA::encrypt(const BigInt& N, const BigInt& e, const char* message) {
    BigInt length = strlen(message);
    BigInt* ciphertext = new BigInt[length];
    bitset<sizeof(BigInt)> encryptor(e);
    size_t lastBit = 0;
    // get last set bit so that there is no need to iterate through leftmost 0s
    for (size_t i = 0; i < encryptor.size(); i++) {
        if (encryptor[i]) lastBit = i;
    }
    unordered_map<char, BigInt> charCodes;
    for (BigInt i = 0; i < length; i++) {
        if (charCodes.find(message[i]) == charCodes.end()) {
            charCodes[message[i]] = powerMod(N, message[i], encryptor, lastBit);
        }
        ciphertext[i] = charCodes.find(message[i])->second;
    }
    return ciphertext;
}

bool RSA::peekFromPool(BigInt& e, const BigInt& phi) {
    for (unsigned int i : ePool)
        if (phi % i != 0 && i % phi != 0) {
            e = i;
            return true;
        }

    return false;
}

BigInt logPowerMod(BigInt x, BigInt n, const BigInt& mod) {
    BigInt y = 1;
    while (n > 0)
        if (n & 0x1u) {
            n--;
            y *= x;
            y %= mod;
        } else {
            n >>= 0x1u;
            x *= x;
            x %= mod;
        }
    return x * y;
}

BigInt RSA::powerMod(const BigInt& N, const BigInt& base, const bitset<sizeof(BigInt)>& exponent, const uint& lastBit) {
    BigInt res = 1;
    map<BigInt, BigInt> powers; // power of 2 - value of number to the power of power of 2
    for (size_t i = 0; i < lastBit; i++) {
        if (exponent[i]) {
            BigInt accumulator = 1;
            auto utilizer = powers.rbegin();
            BigInt currPow = 1ULL << i;
            while (utilizer != powers.rend()) {
                uint mults = currPow / utilizer->first;
                for (uint j = 0; j < mults; j++) {
                    accumulator *= utilizer->second;
                    accumulator %= N;
                }
                assert(currPow%utilizer->first == currPow - mults*utilizer->first);
                currPow %= utilizer->first;
                ++utilizer;
            }
            accumulator *= logPowerMod(base, currPow, N);
            res %= N;
            powers[1ULL << i] = accumulator;
            res *= accumulator;
            res %= N;
        }
    }
    return res;
}
