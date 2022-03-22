/*
 * ReplyBase.cc
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#include "ReplyBase.h"

namespace SF {
namespace CORE {
namespace NET {

ReplyBase::ReplyBase() {
	// TODO Auto-generated constructor stub

}

ReplyBase::~ReplyBase() {
	// TODO Auto-generated destructor stub
}

std::vector<boost::asio::const_buffer> ReplyBase::toBuffers() {
  std::vector<boost::asio::const_buffer> buffers;
  return buffers;
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
