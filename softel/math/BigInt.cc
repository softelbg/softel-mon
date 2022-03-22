/*
 * BigInt.cc
 *
 *  Created on: Mar 18, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include <softel/math/BigInt.h>

namespace SF {
namespace MATH {


void BigInt::set(TNumber n) {
    while (n > 0) {
        n_.push_back(n % prec);
        n = n / prec;
    }
}

void BigInt::trimZeros() {
    while (n_.back() == 0 && n_.size() > 1) {
        n_.pop_back();
    }
}

std::string BigInt::toString() const {
    std::stringstream ss;

    if (n_.empty()) {
        ss << 0;
    }

    for (int i = n_.size() - 1; i >= 0; i--) {
        ss << std::setfill('0') << std::setw(pwr) << n_[i];
    }

    std::string result = ss.str();

    // trim zeros
    while (result.front() == '0') {
        result.erase(result.begin());
    }

    return result;
}

void BigInt::add(const BigInt& bn) {
    TBigInt n;
    bn.get(n);
    add(n);
}

void BigInt::add(const TBigInt& bn) {
    TBigInt n = bn;

    int maxsz = (n_.size() > n.size()) ? n_.size() : n.size();
    for (int i = n_.size(); i < maxsz; i++) {
        n_.push_back(0);
    }
    for (int i = n.size(); i < maxsz; i++) {
        n.push_back(0);
    }

    TNumber c = 0;

    for (size_t i = 0; i < n_.size(); i++) {
        TNumber a = n_[i] + n[i] + c;

        if (a >= prec) {
            a -= prec;
            c = 1;
        } else {
            c = 0;
        }

        n_[i] = a;
    }

    if (c) {
        BigInt b2(c);
        b2.shiftLeft(n_.size());
        add(b2);
    }

    trimZeros();
}

void BigInt::power(int n) {
    BigInt b1(n_);
    BigInt b2(n_);
    for (int i = 1; i < n; i++) {
        b2.mult(b1);
    }

    b2.get(n_);
}

void BigInt::multSingle(const TNumber m) {
    BigInt b2;
    TNumber p = 0;
    TNumber c = 0;
    TNumber r = 0;
    for (size_t i = 0; i < n_.size(); i++) {
        p = m * n_[i] + c;
        c = p / prec;
        r = p % prec;

        BigInt b3(r);
        b3.shiftLeft(i);
        b2.add(b3);
    }

    if (c) {
        BigInt b3(c);
        b3.shiftLeft(n_.size());
        b2.add(b3);
    }

    b2.get(n_);
}

void BigInt::mult(const BigInt& b) {
    TBigInt n;
    b.get(n);

    BigInt bResult;

    int maxsz = (n_.size() > n.size()) ? n_.size() : n.size();

    for (int i = n_.size(); i < maxsz; i++) {
        n_.push_back(0);
    }
    for (int i = n.size(); i < maxsz; i++) {
        n.push_back(0);
    }

    for (size_t i = 0; i < n.size(); i++) {
        BigInt b1(n_);
        b1.multSingle(n[i]);
        b1.shiftLeft(i);
        bResult.add(b1);
    }

    bResult.get(n_);
}

void BigInt::get(TBigInt& n) const {
    n = n_;
}

void BigInt::shiftLeft(int n) {
    for (int i = 0; i < n; i++) {
        n_.insert(n_.begin(), 0);
    }
}

TNumber BigInt::shiftRight() {
    TNumber n = *n_.begin();
    n_.erase(n_.begin());
    return n;
}

size_t BigInt::size() const {
    return n_.size();
}


} /* namespace MATH */
} /* namespace SF */

