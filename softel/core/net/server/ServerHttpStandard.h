/*
 * ServerHttpStandard.h
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#ifndef SERVERHTTPSTANDARD_H_
#define SERVERHTTPSTANDARD_H_

#include "ServerHttpBase.h"

namespace SF {
namespace CORE {
namespace NET {

class ServerHttpStandard: public SF::CORE::NET::ServerHttpBase {
public:
	explicit ServerHttpStandard(const std::string& address, const std::string& port,
					const std::string& doc_root, std::size_t thread_pool_size);
	virtual ~ServerHttpStandard();

private:
	std::string m_doc_root;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* SERVERHTTPSTANDARD_H_ */
