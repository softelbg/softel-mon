/*
 * CCmtsModProfilesList.cc
 *
 *  Created on: 30.05.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsModProfilesList.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;

namespace SF {
namespace OSS {
namespace CMTS {

CCmtsModProfilesList::CCmtsModProfilesList() :
	m_pIp(0) {

}

CCmtsModProfilesList::CCmtsModProfilesList(CIp* ip) {
	setIp(ip);
}

bool CCmtsModProfilesList::initModProfilesList() {

	if (m_pIp) {
		initModProfilesList(m_pIp);
	} else {
		return false;
	}

	return true;
}

/**
 * Initalize Modulation Profile from CMTS via snmp.
 * Fill Modulations List.
 */
bool CCmtsModProfilesList::initModProfilesList(CIp* ip) {

	CDataSet result;
	CDataRow row;

	ip->SnmpWalkBulkIdx2(	"1.3.6.1.2.1.10.127.1.3.5.1.4",
								result);

	//result.buildColumnIndex(KEY_INDEX"1");

	result.resetRowPos();

	while (result.getNextRow(row)) {

		string modProfile;
		string idxModulation;
		string snmpVal;

		row.getColumn(KEY_INDEX"0", idxModulation);
		row.getColumn(KEY_INDEX"1", modProfile);
		row.getColumn(KEY_VALUE, snmpVal);

		CCmtsModProfile* mp;

		if (findModProfile(atoi(modProfile.c_str()), &mp)) {
   
			mp->setModulation(atoi(idxModulation.c_str()), atoi(snmpVal.c_str()));

		} else {

			CCmtsModProfile newModProfile(atoi(modProfile.c_str()));
			newModProfile.setModulation(atoi(idxModulation.c_str()), atoi(snmpVal.c_str()));

			setModProfile(newModProfile);
		}

	}

	return true;
}

void CCmtsModProfilesList::setIp(CIp* ip) {
	m_pIp = ip;
}

/**
 * Add/Set new Modulation Profile
 */
void CCmtsModProfilesList::setModProfile(CCmtsModProfile& mp) {

	int modProfile = mp.getModulationProfile();

	m_modulations[modProfile] = mp;

}

/**
 * Find Mod Profile.
 */
bool CCmtsModProfilesList::findModProfile(int nModProfile, CCmtsModProfile** mp) {

	map<int, CCmtsModProfile>::iterator it;

	it = m_modulations.find(nModProfile);

	if (it != m_modulations.end()) {
		*mp = &(it->second);
		return true;
	} else {
		return false;
	}
}

/**
 * Get Default Modulation as a string from a Modulation Profile (should exists in the list)
 */
void CCmtsModProfilesList::getDefaultModulation(int modprofile, string& result) {

	CCmtsModProfile* mp;

	if (findModProfile(modprofile, &mp)) {
		result = mp->getDefaultModulation();
	} else {
		result = "";
	}

}

void CCmtsModProfilesList::dumpInfo() {

	map<int, CCmtsModProfile>::iterator it;

	cout << __FILE__ << ":pid:" << getpid() << endl;

	for (it = m_modulations.begin(); it != m_modulations.end(); it++) {
		it->second.dumpInfo();
	}

}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
