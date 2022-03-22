/*
 * CShellCmd.cc
 *
 *  Created on: 11.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CShellCmd.h"

using namespace std;

namespace SF {
namespace CORE {
namespace SYSTEM {

CShellCmd::CShellCmd() {

}

CShellCmd::CShellCmd(const string& cmd) :
	m_cmd(cmd) {

}

CShellCmd::~CShellCmd() {
	// TODO Auto-generated destructor stub
}

void CShellCmd::run() {
	execute();
}

/**
 * Execute the command via pipe.
 *
 */
void CShellCmd::execute() {

	if (m_cmd.empty()) {
		return;
	}

	FILE *p;
	char buff[4096];

	m_result.clear();

	p = popen(m_cmd.c_str(), "r");

	while (true) {
		if (!fgets(buff, sizeof(buff), p)) {
			break;
		}

		m_result.append(buff);
	}

	pclose(p);
}

} /* namespace SYSTEM */
} /* namespace CORE */
} /* namespace SF */

