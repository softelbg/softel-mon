/*
 * CCpe.h
 *
 *  Created on: 23.04.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 *
 */

#ifndef CCPE_H_
#define CCPE_H_

#include <map>
#include <vector>
#include <string>
#include <algorithm>

namespace SF {
namespace OSS {
namespace CMTS {

class CCpe {

public:

	CCpe();
	CCpe(const std::string& ip, const std::string& mac);
	~CCpe();

	void setIp(const std::string& ip);
	void setMac(const std::string& mac);

	std::string& getIp();
	std::string& getMac();

private:

	std::string m_ip;
	std::string m_mac;


};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */


#endif /* CCPE_H_ */
