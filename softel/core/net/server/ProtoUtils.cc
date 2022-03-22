/*
 * ProtoUtils.cc
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#include "ProtoUtils.h"
#include <sstream>

namespace SF {
namespace CORE {
namespace NET {

bool ProtoUtils::urlDecode(const std::string& in, std::string& out) {
	out.clear();
	out.reserve(in.size());
	for (std::size_t i = 0; i < in.size(); ++i) {
		if (in[i] == '%') {
			if (i + 3 <= in.size()) {
				int value = 0;
				std::istringstream is(in.substr(i + 1, 2));
				if (is >> std::hex >> value) {
					out += static_cast<char>(value);
					i += 2;
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else if (in[i] == '+') {
			out += ' ';
		} else {
			out += in[i];
		}
	}
	return true;
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
