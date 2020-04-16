//
// Created by Andrew on 16-Apr-20.
//

#ifndef PROJECTOOP_PRIMEMINER_H
#define PROJECTOOP_PRIMEMINER_H


#include <set>
using BigInt = unsigned long long;

class PrimeMiner {
private:
    static PrimeMiner* _instance;

    const BigInt primeBase = 5;
    const BigInt circlePoint = 2;
    const BigInt primeStep = 6;

    BigInt _lastBound = 0;

    std::set<BigInt> _primes;
    PrimeMiner() = default;
public:
    void fillPrimesUpTo(BigInt top);

    const std::set<BigInt>& currPrimes() const;

    static PrimeMiner& getInstance();

    static void destruct();

    PrimeMiner(const PrimeMiner&) = delete;
};


#endif //PROJECTOOP_PRIMEMINER_H
