/*
 * BigInt.h
 *
 *  Created on: Mar 18, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_CORE_UTIL_BIGINT_H_
#define SOFTEL_CORE_UTIL_BIGINT_H_

#include <softel/core/common.h>

namespace SF {
namespace MATH {


typedef unsigned long long TNumber;
typedef std::vector<TNumber> TBigInt;

int pwr = 9;
unsigned long long prec = pow(10, pwr);

class BigInt {
public:
    BigInt() {
        set(0);
    }
    BigInt(TNumber n) {
        set(n);
    }
    BigInt(const TBigInt& n) {
        n_ = n;
    }

    virtual ~BigInt() {}

    void set(TNumber n);
    void add(const BigInt& bn);
    void add(const TBigInt& bn);
    void power(int n);
    void multSingle(const TNumber m);
    void mult(const BigInt& b);
    void get(TBigInt& n) const;
    void shiftLeft(int n = 1);
    TNumber shiftRight();

    size_t size() const;

    void trimZeros();
    std::string toString() const;

private:
    TBigInt n_;
};


} /* namespace MATH */
} /* namespace SF */

#endif /* SOFTEL_CORE_UTIL_BIGINT_H_ */
