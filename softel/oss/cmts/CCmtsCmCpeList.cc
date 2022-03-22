/*
 * CCmtsCmCpeList.cc
 *
 *  Created on: 22.04.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsCmCpeList.h"

using namespace std;


namespace SF {
namespace OSS {
namespace CMTS {

CCmtsCmCpeList::CCmtsCmCpeList() {

	clear();

}

CCmtsCmCpeList::~CCmtsCmCpeList() {

}

void CCmtsCmCpeList::clear() {

	m_cpe_list.clear();
}

int CCmtsCmCpeList::size() {

	return m_cpe_list.size();
}

void CCmtsCmCpeList::addCpe(const CCpe& cpe) {
	m_cpe_list.push_back(cpe);
}

void CCmtsCmCpeList::addCpe(const std::string& ip, const std::string& mac) {

	CCpe cpe(ip, mac);

	addCpe(cpe);

}

CCpe& CCmtsCmCpeList::getCpe(const int pos) {

	return m_cpe_list[pos];
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

