/*
 * common.h
 *
 *  Created on: Mar 22, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_MATH_COMMON_H_
#define SOFTEL_MATH_COMMON_H_

namespace SF {
namespace MATH {

class Util {
public:

    static int gcd(int a, int b) {
        int r = a % b;
        if (r == 0) return b;
        return gcd(b, r);
    }

    static void sieve(vector<bool>& sieve, int n) {
        sieve.resize(n, true);

        sieve[0] = false;
        sieve[1] = false;

        int i = 2;
        int k = 0;

        // sieve init
        while (i * i <= n) {
            if (sieve[i]) {
                k = i * i;
            }

            while (k <= n) {
                sieve[k] = false;
                k += i;
            }

            i++;
        }
    }

};

} /* namespace MATH */
} /* namespace SF */

#endif /* SOFTEL_MATH_COMMON_H_ */
