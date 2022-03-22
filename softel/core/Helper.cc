/*
 * Helper.cc
 *
 *  Created on: Apr 27, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include <softel/core/Helper.h>

using namespace std;

namespace SF {
namespace CORE {

Helper::Helper() {
    // TODO Auto-generated constructor stub

}

Helper::~Helper() {
    // TODO Auto-generated destructor stub
}

std::string Helper::toHex(const unsigned char* pData, size_t len, size_t line/* = 16*/) {
    std::stringstream ssHex;
    std::stringstream ssASCII;

    ssHex << std::endl;
    size_t i = 0;

    for (i = 0; i < len; ++i) {
        ssHex << std::hex << std::setfill('0') << std::setw(2) << (unsigned short)pData[i] << " ";
        char c = (char)pData[i];
        ssASCII << ((c >= 32 && c <= 127) ? c : '.');

        if ((i + 1) % line == 0) {
            ssHex << "  " << ssASCII.str() << std::endl;
            ssASCII.str(std::string());
        }
    }

    if (!ssASCII.str().empty()) {
        ssHex << std::string(3 * (line - i % line), '_') << "  " << ssASCII.str() << std::endl;
    }

    return ssHex.str();
}


} /* namespace CORE */
} /* namespace SF */
