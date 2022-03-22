/*
 * ProtoUtils.h
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#ifndef PROTOUTILS_H_
#define PROTOUTILS_H_

#include <string>

namespace SF {
namespace CORE {
namespace NET {

class ProtoUtils {
public:

	static bool urlDecode(const std::string& in, std::string& out);

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* PROTOUTILS_H_ */
