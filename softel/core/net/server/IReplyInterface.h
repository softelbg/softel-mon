/*
 * IReplyInterface.h
 *
 *  Created on: Dec 6, 2013
 *      Author: sgeorgiev
 */

#ifndef IREPLYINTERFACE_H_
#define IREPLYINTERFACE_H_

#include <boost/asio.hpp>

namespace SF {
namespace CORE {
namespace NET {

class IReplyInterface {

	virtual std::vector<boost::asio::const_buffer> toBuffers() = 0;

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* IREPLYINTERFACE_H_ */
