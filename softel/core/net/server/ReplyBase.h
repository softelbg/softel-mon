/*
 * ReplyBase.h
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#ifndef REPLYBASE_H_
#define REPLYBASE_H_

#include <vector>
#include <boost/asio.hpp>
#include "IReplyInterface.h"

namespace SF {
namespace CORE {
namespace NET {

class ReplyBase : virtual public IReplyInterface {
public:
	ReplyBase();
	virtual ~ReplyBase();

	virtual std::vector<boost::asio::const_buffer> toBuffers();

	const std::string& getReply() const {
		return m_reply;
	}

	void setReply(const std::string& reply) {
		m_reply = reply;
	}

private:
	std::string m_reply;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* REPLYBASE_H_ */
