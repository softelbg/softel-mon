/*
 * CCmtsModProfile.h
 *
 *	CMTS Modulation Profile - should contain cmts available modulations for a profile
 *
 *  Created on: 30.05.2012
 *      Author: STAN
 *
 *     www.softel.bg
 *
 *     Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSMODPROFILE_H_
#define CCMTSMODPROFILE_H_

#include "CMTSMonCommon.h"

namespace SF {
namespace OSS {
namespace CMTS {

class CCmtsModProfile {

public:

	CCmtsModProfile();
	CCmtsModProfile(int modprofile);

	void setModulationProfile(int modprofile);
	void setModulation(int modidx, int modulation);

	std::string& getDefaultModulation(); // Get modulation as a string (10-th modulation in profile)
	int 	getModulationProfile();

	void dumpInfo();

private:

	void initModTypes();

	int		m_profile;
	std::map<int, int> m_modulations;

	std::map<int, std::string> m_modTypes;
};


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSMODPROFILE_H_ */
