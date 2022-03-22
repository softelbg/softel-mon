/*
 * CCmtsModProfile.cc
 *
 *  Created on: 30.05.2012
 *      Author: STAN
 *
 *  	www.softel.bg
 *
 *  	Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsModProfile.h"

using namespace std;

namespace SF {
namespace OSS {
namespace CMTS {

CCmtsModProfile::CCmtsModProfile() {

	initModTypes();

}

CCmtsModProfile::CCmtsModProfile(int modprofile) {

	setModulationProfile(modprofile);

	initModTypes();

}

/**
 * Initialize Modulation Types
 * TODO: Consider read these types from DB
 */
void CCmtsModProfile::initModTypes() {

	m_modTypes.clear();

	m_modTypes[1] = "other";
	m_modTypes[2] = "qpsk";
	m_modTypes[3] = "qam16";
	m_modTypes[4] = "qam8";
	m_modTypes[5] = "qam32";
	m_modTypes[6] = "qam64";
	m_modTypes[7] = "qam128";

}

void CCmtsModProfile::setModulationProfile(int modprofile) {
	m_profile = modprofile;
}

/**
 * Set/Add modulation in the Profile.
 *
 */
void CCmtsModProfile::setModulation(int modidx, int modulation) {
	m_modulations[modidx] = modulation;
}

/**
 * Get Modulation used most in the communication - 10-th is for now...
 *
 */
string& CCmtsModProfile::getDefaultModulation() {

	int modulation;

	modulation = m_modulations[10];

	return m_modTypes[modulation];
}

int CCmtsModProfile::getModulationProfile() {
	return m_profile;
}

void CCmtsModProfile::dumpInfo() {

	map<int, int>::iterator it;

	cout << __FILE__ << ":pid:" << getpid();
	cout << " ModProfile: " << m_profile << endl;

	for(it = m_modulations.begin(); it != m_modulations.end(); it++) {
		cout << "\tmod[" << it->first << "] = " << it->second;
	}


}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

