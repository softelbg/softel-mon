/*
 * CServerInfo.cc
 *
 *  Created on: 30.01.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CServerInfo.h"

using namespace std;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace EXEC {

CServerInfo::CServerInfo() {
	this->init();
	this->executeList();
}

CServerInfo::~CServerInfo() {
	// TODO Auto-generated destructor stub
}

void CServerInfo::init() {

	this->initCmdList();

}

void CServerInfo::executeList() {
	for(size_t i = 0; i < m_cmd_list.size(); i++) {
		m_cmd_list[i].run();
	}
}

/**
 * Get Shell CMD Result by position from cmd list.
 */
string CServerInfo::getResult(int pos) const {

	string result;

	if(validatePos(pos)) {
		result = this->m_cmd_list[pos].getResult();
	}

	return result;
}

void CServerInfo::clear() {
	this->m_cmd_list.clear();
}

int CServerInfo::size() const {
	return this->m_cmd_list.size();
}

bool CServerInfo::validatePos(int pos) const {

	if(pos >= 0 and pos < this->size()) {
		return true;
	} else {
		return false;
	}
}

std::string CServerInfo::getResult() const {

	std::string result;

	for (int i = 0; i < this->size(); i++) {

		result += this->getResult(i);

		result += "\n";
	}

	return result;
}

/**
 * Initialize the CMD List.
 * TODO: Could initialize from DB or a ini file.
 * Now it is initialized here...
 */
void CServerInfo::initCmdList() {

	this->clear();

	this->m_cmd_list.push_back(CShellCmd("df -h"));
	this->m_cmd_list.push_back(CShellCmd("ps -afe|grep Cmts|grep -v grep"));
	this->m_cmd_list.push_back(CShellCmd("top -b"));
	this->m_cmd_list.push_back(CShellCmd("/usr/local/sbin/arcconf getconfig 1|grep -E -i -w \"Group|State\""));

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


