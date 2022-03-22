/*
 * Helper.h
 *
 *  Created on: Apr 27, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_CORE_HELPER_H_
#define SOFTEL_CORE_HELPER_H_

#include <softel/core/common.h>

namespace SF {
namespace CORE {

class Helper {
public:
    Helper();
    virtual ~Helper();

    template<class K, class V>
    static V get(const std::map<K, V>& m, const K& k) {
        V result;
        auto it = m.find(k);
        if (it != m.end()) {
            result = it->second;
        }
        return result;
    }

    static std::string toHex(const unsigned char* pData, size_t len, size_t line = 16);
};

} /* namespace CORE */
} /* namespace SF */

#endif /* SOFTEL_CORE_HELPER_H_ */
