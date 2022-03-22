/*
 * CCmtsModProfilesList.h
 *
 *  Created on: 30.05.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSMODPROFILESLIST_H_
#define CCMTSMODPROFILESLIST_H_

#include "CMTSMonCommon.h"
#include "CCmtsModProfile.h"
#include <softel/core/net/CIp.h>

namespace SF {
namespace OSS {
namespace CMTS {

class CCmtsModProfilesList {

public:

	CCmtsModProfilesList();
	CCmtsModProfilesList(SF::CORE::NET::CIp* ip);

	void setIp(SF::CORE::NET::CIp* ip);

	void setModProfile(CCmtsModProfile& modprofile);

	void getDefaultModulation(int modprofile, std::string& result);

	bool findModProfile(int, CCmtsModProfile**);

	bool initModProfilesList();
	bool initModProfilesList(SF::CORE::NET::CIp* ip);

	void dumpInfo();

private:

	std::map<int, CCmtsModProfile> m_modulations;

	SF::CORE::NET::CIp* 	m_pIp;

};


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */


#endif /* CCMTSMODPROFILESLIST_H_ */
