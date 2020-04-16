//
// Created by Andrew on 16-Apr-20.
//

#include "PrimeMiner.h"
using namespace std;

PrimeMiner* PrimeMiner::_instance = nullptr;

void PrimeMiner::fillPrimesUpTo(BigInt top) {
    BigInt d = primeBase;
    if (_lastBound == 0) {
        _primes.insert(2);
        _primes.insert(3);
    } else while (d + primeStep <= _lastBound) d += primeStep;
    // add primes and pseudo-primes
    while (d <= top) {
        _primes.insert(d);
        if (d + circlePoint <= top) _primes.insert(d + circlePoint);
        d += primeStep;
    }
    //filter pseudo-primes
    for (BigInt i = primeBase; i * i <= top; i += primeStep) {
        BigInt j = 0;

        bool overflow1 = false;
        bool overflow2 = false;
        bool overflow3 = false;
        bool overflow4 = false;

        while (true) {
            if (!overflow1) {
                BigInt pseudo1 = primeStep * i * (j + 1) + i;
                if (pseudo1 > top) overflow1 = true;
                else _primes.erase(pseudo1);
            }

            if (!overflow2) {
                BigInt pseudo2 = primeStep * i * j + i * i;
                if (pseudo2 > top) overflow2 = true;
                else _primes.erase(pseudo2);
            }

            if (!overflow3) {
                BigInt pseudo3 = primeStep * (i + circlePoint) * j + (i + circlePoint) * (i + circlePoint);
                if (pseudo3 > top) overflow3 = true;
                else _primes.erase(pseudo3);
            }

            if (!overflow4) {
                BigInt pseudo4 = primeStep * (i + circlePoint) * (j + 1) + (i + circlePoint) * (i + circlePoint)
                                 - circlePoint * (i + circlePoint);
                if (pseudo4 > top) overflow4 = true;
                else _primes.erase(pseudo4);
            }

            if (overflow1 && overflow2 && overflow3 && overflow4) break;
            j++;
        }
    }
    _lastBound = top;
}

const std::set<BigInt>& PrimeMiner::currPrimes() const {
    return _primes;
}

PrimeMiner& PrimeMiner::getInstance() {
    return _instance == nullptr ? *(_instance = new PrimeMiner()) : *_instance;
}

void PrimeMiner::destruct() {
    delete _instance;
    _instance = nullptr;
}
